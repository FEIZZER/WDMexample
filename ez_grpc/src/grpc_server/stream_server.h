#pragma once
#include "base.grpc.pb.h"

#include <memory>

namespace ez_grpc {

	class StreamServer
	{
	public:
		StreamServer() = default;
		virtual ~StreamServer() = default;

		bool Init();

		void UnInit();

	private:
		void DrawFromCompletionQueue(std::unique_ptr<grpc::ServerCompletionQueue> cq);

	private:
		Base::AsyncService* async_service_;
		std::unique_ptr<grpc::Server> server_;

		int thread_num_;
		std::vector<std::unique_ptr<grpc::ServerCompletionQueue>> cqs_;
		std::vector<std::thread> threads_;

		bool exit_;
	};
}