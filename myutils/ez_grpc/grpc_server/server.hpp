#pragma once
#include "ez_grpc/server/server.h"

#include "server_connect.hpp"
#include "ez_thread_pool/ez_thread_pool.h"

#include "base.grpc.pb.h"
#include "grpcpp/grpcpp.h"
#include <thread>

namespace ez_grpc {

	using async_base_service = Base::AsyncService;
	using async_generic_service = grpc::AsyncGenericService;
	using server_cq = grpc::ServerCompletionQueue;
	using server_cq_status = grpc::ServerCompletionQueue::NextStatus;

	class sync_service_impl : public Base::Service
	{
	public:

		sync_service_impl(ez_event_handler handler) : handler_(handler) {}
		~sync_service_impl() = default;

		grpc::Status BaseTransmit(grpc::ServerContext* context, const BaseRequest* request, BaseReply* response) override
		{
			auto request_length = request->length();
			auto request_buffer = request->buffer().c_str();

			void* reply_buffer = nullptr;
			int	reply_length = 0;

			if (handler_ == nullptr)
			{	//  ERROR
				return grpc::Status::OK;
			}

			handler_((void*)request_buffer, request_length, &reply_buffer, &reply_length);

			if (reply_buffer != nullptr && reply_length != 0)
			{
				response->set_length(reply_length);
				response->set_buffer(reply_buffer, reply_length);
				delete[] reply_buffer;
			}
			else
			{
				response->set_length(8);
				response->set_buffer("no reply");
			}

			return grpc::Status::OK;
		}

	private:
		ez_event_handler handler_;
	};
	class server_impl : public server
	{
	public:
		server_impl() = default;
		virtual ~server_impl()
		{
			exit_.store(true);
			cq_worker_.join();
			
		}

		bool Start(ez_event_handler handler, std::string& ip, std::string& port)
		{
			if (handler == nullptr) return false;
			sync_service_ = new sync_service_impl(handler);
			async_generic_service_ = new grpc::AsyncGenericService() ;
			if (sync_service_ == nullptr || async_generic_service_ == nullptr) return false;
			int selected_port = 0;
			grpc::ServerBuilder builder;
			builder.AddChannelArgument(GRPC_ARG_ALLOW_REUSEPORT, 0);
			builder.RegisterService(sync_service_);
			builder.RegisterAsyncGenericService(async_generic_service_);
			builder.AddListeningPort(ip + port, grpc::InsecureServerCredentials(), &selected_port);
			cq_ = builder.AddCompletionQueue();
			if (cq_ == nullptr) return false;
			cq_worker_ = std::thread(&server_impl::DrawEventFromCompetionQueue, this);
			server_ = builder.BuildAndStart();
			if (server_ == nullptr || selected_port == 0)return false;
			port = std::to_string(selected_port);
			return true;
		}

	private:
		void DrawEventFromCompetionQueue()
		{
			if (cq_ == nullptr) return;
			void* tag;
			bool ok;
			server_cq_status status;


			while (!exit_.load()) {

				tag = nullptr, ok = false;
				status = cq_->AsyncNext(&tag, &ok, std::chrono::system_clock::now() + std::chrono::milliseconds(2000));

				switch (status)
				{
				case grpc::CompletionQueue::SHUTDOWN:
					exit_.store(true);
					break;

				case grpc::CompletionQueue::GOT_EVENT:
					auto connect = (server_connect_impl*)tag;
					if (connect == nullptr) exit_.store(true);
					connect->Proceed();
					break;

				case grpc::CompletionQueue::TIMEOUT:
					break;

				default:
					break;
				}
			}
		}

	private:
		async_base_service*		async_base_service_;
		async_generic_service*	async_generic_service_;
		sync_service_impl*		sync_service_;
		std::unique_ptr<grpc::Server> server_;
		std::unique_ptr<server_cq> cq_;

		std::thread				cq_worker_;
		std::atomic_bool		exit_;
	};
}