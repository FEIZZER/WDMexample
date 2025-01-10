#include "server.h"


#include <grpcpp/grpcpp.h>

using namespace ez;

bool server::start_server(const std::string& ip, unsigned int port, std::shared_ptr<ServerCredentials> server_cred)
{
	auto server_address = ip + ":" + std::to_string(port);
	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, server_cred);
	builder.RegisterService(&client_stream_service_);

	grpc_server_ = builder.BuildAndStart();
	if (grpc_server_ == nullptr)
	{
		EZ_ERROR("BuildAndStart failed, server_address:{}", server_address);
		return false;
	}

	return true;
}