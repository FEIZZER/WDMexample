#include "service.h"


using namespace ez_grpc;

#include <iostream>
grpc::Status BasicServiceImpl::BaseTransmit(grpc::ServerContext* context, const ::BaseRequest* request, BaseReply* response)
{
	auto request_length = request->length();
	auto request_buffer = request->buffer().c_str();

	std::cout << (char*)request_buffer << std::endl;

	return grpc::Status::OK;
}