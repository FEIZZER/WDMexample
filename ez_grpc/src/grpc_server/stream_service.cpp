#include "stream_service.h"

#include <iostream>

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
		std::cout << (ULONG_PTR)this << " get connected, cq:" << (ULONG_PTR)cq_ << std::endl;
		connected_ = true;
		context_.AsyncNotifyWhenDone(this);
		new ServerConnect(async_service_, cq_);
	}

	if (context_.IsCancelled())
	{
		std::cout <<  (ULONG_PTR)this << " disconnected from client" << std::endl;
		delete this;
		return;
	}

	stream_.Read(&request_, this);

	auto value = request_.buffer().c_str();
	


	request_.Clear();
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