#include "ez_grpc/client/client.h"

#include <mutex>
#include <shared_mutex>

#include "client_connect_internal.h"

using namespace ez_grpc;

Client::Client()
{

}

Client::~Client()
{

}


std::shared_ptr<ClientConnectInterface> Client::CreateShortConnection(const std::string& key, const std::string& ip, int port)
{
	auto connection = CreateConnectionInternal(false, ip, port);
	if (connection == nullptr)
	{
		return nullptr;
	}

	std::shared_lock<std::shared_mutex> lock(shared_mutex_);
	//todo: if key already exist, warning
	connection_pool_[key] = connection;

	return connection;
}


std::shared_ptr<ClientConnectInterface> Client::CreateLongConnection(const std::string& key, const std::string& ip, int port)
{
	auto connection = CreateConnectionInternal(true, ip, port);
	if (connection == nullptr)
	{
		return nullptr;
	}

	std::shared_lock<std::shared_mutex> lock(shared_mutex_);
	//todo: if key already exist, warning
	connection_pool_[key] = connection;

	return connection;
}


std::shared_ptr<ClientConnectInterface> Client::CreateTemporaryConnection()
{

}


std::shared_ptr<ClientConnectInterface> Client::CreateConnectionInternal(bool keepalive, const std::string& ip, int port)
{
	
	if (keepalive)
	{
		return std::make_shared<ClientConnectStream>(ip, port);
	}
	else
	{
		return std::make_shared<ClientConnectInternal>(ip, port);
	}
}