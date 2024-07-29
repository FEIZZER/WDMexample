#include "basic_service.h"


using namespace grpc_cpp_test;

#include <iostream>
grpc::Status BasicServiceImpl::BaseTransmit(grpc::ServerContext* context, const ::BaseRequest* request, BaseReply* response)
{
	auto request_length = request->length();
	auto request_buffer = request->buffer().c_str();

	std::cout << (char*)request_buffer << std::endl;

	while (true)
	{
		std::cout << "waiting" << std::endl;
		Sleep(5000);
	}

	return grpc::Status::OK;
}