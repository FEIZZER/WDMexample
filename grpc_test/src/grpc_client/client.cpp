#include "client.h"


using namespace grpc_cpp_test;

#include <iostream>

bool Client::Request(int port, const void* buffer, const int length)
{
	BaseReply reply;
	BaseRequest request;
	request.set_length(length);
	request.set_buffer(buffer, length);

	auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(5);
	grpc::ClientContext client_ctx;
	client_ctx.set_deadline(deadline);

	auto channel = grpc::CreateChannel("127.0.0.1:" + std::to_string(port), grpc::InsecureChannelCredentials());
	stub_ = Base::NewStub(channel);
	grpc::Status status = stub_->BaseTransmit(&client_ctx, request, &reply);

	std::cout << "BaseTransmit Done. " << status.error_code() << std::endl;
	return true;
}