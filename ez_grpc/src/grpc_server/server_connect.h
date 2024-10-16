#pragma once
#include "ez_grpc/server/server_connect.h"

#include "base.grpc.pb.h"
#include "ez_grpc/common/status.h"

namespace ez_grpc {

	class ServerConnectImpl : public ServerConnect
	{

	public:

	public:
		ServerConnectImpl() = delete;
		virtual ~ServerConnectImpl() = default;

		ServerConnectImpl(Base::AsyncService* async_service, grpc::ServerCompletionQueue* cq);

		EZCode Proceed(BaseRequest& reuqest);

		void DisConnect();

		bool Write(void* buffer, int length);

	private:
		enum class Status {
			Created = 0,
			Connected,
			DisConnected
		};

	private:
		ServerConnectImpl::Status connect_status_;
		std::string client_connect_uid_;

		Base::AsyncService* async_service_;
		grpc::ServerCompletionQueue* cq_;

		grpc::ServerContext context_;
		grpc::ServerAsyncReaderWriter<BaseReply, BaseRequest> stream_;

		BaseRequest request_;

	};
}