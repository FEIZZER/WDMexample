#pragma once
#include "base.grpc.pb.h"
#include "grpc/grpc.h"
#include "server_connect.h"

#include <memory>
#include <mutex>
#include <shared_mutex>

namespace ez_grpc {

	class StreamServer
	{
	public:
		StreamServer() = default;
		virtual ~StreamServer();

		bool Init();

		bool Start(const std::string& addr, int thread_nums, int max_connect);

		void Shutdown();

		void NewConnect(grpc::ServerCompletionQueue* cq);

		std::shared_ptr<ServerConnectImpl> GetConnect(const std::string& key);

		bool DeleteConnect(const std::string& key);

	private:
		bool StartInternal(const std::string& addr, bool limit, int thread_nums);

		bool RunWithLimit(int thread_nums);

		bool Run();

		void DrawFromCq(grpc::ServerCompletionQueue* cq);

		void DrawFromCqWithOneConnect(grpc::ServerCompletionQueue* cq);

	private:
		Base::AsyncService* async_service_;
		std::unique_ptr<grpc::Server> server_;

		int thread_num_;
		std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> cqs_;
		std::vector<std::thread> threads_;

		std::shared_mutex shared_mutex_;
		std::map<std::string, std::shared_ptr<ServerConnectImpl>> connect_pool_;
		
		bool exit_;
	};
}