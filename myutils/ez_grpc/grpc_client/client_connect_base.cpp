#include "client_connect_base.h"
#include "ez/log.hpp"
using namespace ez;

client_connect_base::~client_connect_base()
{
	stub_.reset();
}

client_connect_base::client_connect_base(unsigned long port) : client_connect_base("0.0.0.0", port){}


client_connect_base::client_connect_base(const std::string& ip, unsigned long port)
{
	connected_.store(false);
	std::string addr = ip + ":" + std::to_string(port);
	auto channel = grpc::CreateChannel(addr, grpc::InsecureChannelCredentials());
	if (channel == nullptr)
	{
		EZ_ERROR("CreateChannel failed, addr:{}", addr);
		return;
	}

	auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(2000);
	if (!channel->WaitForConnected(deadline))
	{
		EZ_ERROR("WaitForConnected failed, addr:{}", addr);
		return;
	}

	stub_ = Communication::NewStub(channel);
	if (stub_ == nullptr)
	{
		EZ_ERROR("Communication::NewStub failed, addr:{}", addr);
		return;
	}

	target_ip_ = ip;
	target_port_ = port;
	connected_.store(true);
}

bool client_connect_base::is_connected()
{
	return connected_.load();
}



void test(grpc::Status status)
{

}

bool client_connect_base::Request(const Package& request, Package& response)
{
	grpc::ClientContext context;
	stub_->async()->UnaryCommunicate(&context, &request, &response, test);
	return true;
}