#pragma once
#include "base.grpc.pb.h"


namespace ez_grpc 
{
	using ez_request_handler = void (*)(void* buffer, int length, void** out_buffer, int* out_length);

	class BasicServiceImpl final : public Base::Service
	{
	public:
		BasicServiceImpl(ez_request_handler handler);
		virtual ~BasicServiceImpl() = default;

		grpc::Status BaseTransmit(grpc::ServerContext* context, const ::BaseRequest* request, BaseReply* response) override;

	private:
		ez_request_handler handler_;
	};

}