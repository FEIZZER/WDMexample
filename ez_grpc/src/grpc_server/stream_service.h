#pragma once
#include "base.grpc.pb.h"

namespace ez_grpc {

	class ServerConnect
	{
	public:
		ServerConnect() = delete;
		virtual ~ServerConnect() = default;

		ServerConnect(Base::AsyncService* async_service, grpc::ServerCompletionQueue* cq);

		void Proceed();

		bool ReplyData(void* buffer, int length);

	private:
		bool connected_;

		Base::AsyncService* async_service_;
		grpc::ServerCompletionQueue* cq_;

		grpc::ServerContext context_;
		grpc::ServerAsyncReaderWriter<BaseReply, BaseRequest> stream_;

		BaseRequest request_;
	};
}