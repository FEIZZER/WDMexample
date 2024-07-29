#include "server.h"

#include <grpcpp/grpcpp.h>

using namespace grpc_cpp_test;

#define LocalHost		"127.0.0.1:"
#define RandomAddr		"127.0.0.1:0"

BasicServer::BasicServer()
{

}

BasicServer::~BasicServer()
{

}

int BasicServer::BuildServer()
{
	int selected_port = 0;
	if (!BuildServerInternal(RandomAddr, selected_port))
	{

	}
	return selected_port;
}

bool BasicServer::BuildServer(const int port)
{
	int selected_port = 0;
	if (!BuildServerInternal(LocalHost + std::to_string(port), selected_port))
	{
		std::cout << "failed";
	}
	return true;
}

bool BasicServer::BuildServerInternal(const std::string& addr_uri, int& selected_port)
{
	bool bRet = false;
	if (basic_service_ != nullptr)
	{
		return bRet;
	}

	do
	{
		basic_service_ = new BasicServiceImpl();
		if (basic_service_ == nullptr)
		{
			break;
		}

		grpc::ServerBuilder builder;
		builder.AddChannelArgument(GRPC_ARG_ALLOW_REUSEPORT, 0);
		builder.RegisterService(basic_service_);
		builder.AddListeningPort(addr_uri, grpc::InsecureServerCredentials(), &selected_port);
		server_ = builder.BuildAndStart();

		if (server_ == nullptr)
		{
			break;
		}

		bRet = true;

	} while (0);

	if (!bRet && basic_service_ != nullptr)
	{
		delete basic_service_;
		basic_service_ = nullptr;
	}

	return bRet;
}