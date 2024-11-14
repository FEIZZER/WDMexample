#pragma once
#include "ez_grpc/server/server_connect.h"

#include "grpcpp/grpcpp.h"
#include "grpcpp/generic/async_generic_service.h"

namespace ez_grpc 
{
	class server_connect_impl : public sever_connect
	{
	public:
		using generic_server_ctx = grpc::GenericServerContext;
		using generic_server_async_rw = grpc::GenericServerAsyncReaderWriter;
		using async_generic_service = grpc::AsyncGenericService;
		using server_cq = grpc::ServerCompletionQueue;

		server_connect_impl(async_generic_service* service, server_cq* cq)
			: service_(service), cq_(cq), rw_(&context_)
		{
			status_ = connect_status::CREATE;
			context_.AsyncNotifyWhenDone(this);
			service_->RequestCall(&context_, &rw_, cq, cq, this);
		};

		connect_status Proceed() 
		{	
			switch (status_)
			{
			case ez_grpc::connect::connect_status::CREATE:

				new server_connect_impl(service_, cq_);

				status_ = connect_status::PROCESS;

				break;
			case ez_grpc::connect::connect_status::PROCESS:

				rw_.Read(&request_, this);

				break;
			case ez_grpc::connect::connect_status::DISCONNECT:
				break;
			case ez_grpc::connect::connect_status::FINISH:
				break;
			case ez_grpc::connect::connect_status::MAX:
				break;
			default:
				break;
			}


		}

		~server_connect_impl();

	private:
		async_generic_service*		service_;
		generic_server_ctx			context_;
		generic_server_async_rw		rw_;
		server_cq*					cq_;

		grpc::ByteBuffer			request_;
	};
}