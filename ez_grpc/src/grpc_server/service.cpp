#include "service.h"


using namespace ez_grpc;

#include <iostream>


BasicServiceImpl::BasicServiceImpl(ez_request_handler handler)
{
	if (handler == nullptr)
	{
		// warning
	}

	handler_ = handler;
}


grpc::Status BasicServiceImpl::BaseTransmit(grpc::ServerContext* context, const ::BaseRequest* request, BaseReply* reply)
{
	auto request_length = request->length();
	auto request_buffer = request->buffer().c_str();

	void* reply_buffer = nullptr;
	int	reply_length = 0;

	if (handler_ == nullptr)
	{	//  ERROR
		return grpc::Status::OK;
	}

	//todo: ·â×° request replyµÄbuffer
	handler_((void*)request_buffer, request_length, &reply_buffer, &reply_length);

	if (reply_buffer != nullptr && reply_length != 0)
	{
		reply->set_length(reply_length);
		reply->set_buffer(reply_buffer, reply_length);
		delete[] reply_buffer;
	}
	else 
	{
		reply->set_length(8);
		reply->set_buffer("no reply");
	}



	return grpc::Status::OK;
}