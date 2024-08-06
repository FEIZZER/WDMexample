#include "stream_service.h"

using namespace ez_grpc;

ServerConnect::ServerConnect(Base::AsyncService* async_service, grpc::ServerCompletionQueue* cq) : stream_(&context_)
{
	cq_ = cq;
	async_service_ = async_service;

	async_service_->RequestStreamTransmit(&context_, &stream_, cq_, cq_, this);
	connected_ = false;
}

void ServerConnect::Proceed()
{
	// get connected
	if (!connected_)
	{
		connected_ = true;
		new ServerConnect(async_service_, cq_);
	}

	BaseRequest request;
	stream_.Read(&request, this);
	auto value = request.buffer().c_str();
	std::cout << "request_value: " << value << std::endl;
}


bool ServerConnect::ReplyData(void* buffer, int length)
{
	if (buffer == nullptr || length == 0)
	{
		return false;
	}

	BaseReply reply;
	reply.set_length(length);
	reply.set_buffer(buffer, length);
	stream_.Write(reply, this);

	return true;
}