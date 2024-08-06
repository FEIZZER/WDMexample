#include "server.h"

#include <grpcpp/grpcpp.h>

using namespace ez_grpc;

#define LocalHost		"127.0.0.1:"
#define RandomAddr		"127.0.0.1:0"

Server::Server()
{

}

Server::~Server()
{

}

int Server::BuildServer(ez_request_handler handler)
{
	int selected_port = 0;

	if (!BuildServerInternal(RandomAddr, selected_port, handler))
	{

	}
	return selected_port;
}

bool Server::BuildServer(const int port, ez_request_handler handler)
{
	int selected_port = 0;

	if (!BuildServerInternal(LocalHost + std::to_string(port), selected_port, handler))
	{
		
	}
	return true;
}

bool Server::BuildServerInternal(const std::string& addr_uri, int& selected_port, ez_request_handler handler)
{
	bool bRet = false;

	if (handler == nullptr)
	{
		return bRet;
	}

	if (basic_service_ != nullptr)
	{
		return bRet;
	}

	do
	{
		basic_service_ = new BasicServiceImpl(handler);
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