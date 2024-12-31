#include "client_connect_base.h"

using namespace ez_grpc;

client_connect_base::~client_connect_base()
{
}

client_connect_base::client_connect_base(unsigned long port) : client_connect_base("0.0.0.0", port){}


client_connect_base::client_connect_base(const std::string& ip, unsigned long port)
{
	std::string addr = ip + ":" + std::to_string(port);
	auto channel = grpc::CreateChannel(addr, grpc::InsecureChannelCredentials());
	if (channel == nullptr)
	{
		return;
	}

	stub_ = Communication::NewStub(channel);
	if (stub_ == nullptr)
	{
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