#pragma once

#include <mutex>
#include <shared_mutex>
#include <memory>
#include <map>

#include "ez_grpc/common/connect.h"
#include "ez_grpc/client/client_connect.h"

namespace ez_grpc {

	class Client
	{
	public:
		Client();
		~Client();

		std::shared_ptr<ClientConnectInterface> CreateShortConnection(const std::string& key, const std::string& ip, int port);

		std::shared_ptr<ClientConnectInterface> CreateLongConnection();

		std::shared_ptr<ClientConnectInterface> CreateTemporaryConnection();

	private:
		std::shared_ptr<ClientConnectInterface> CreateConnectionInternal(const std::string& ip, int port);

	private:
		std::shared_mutex shared_mutex_;
		std::map<std::string, std::shared_ptr<ClientConnectInterface>> connection_pool_;
	};
}