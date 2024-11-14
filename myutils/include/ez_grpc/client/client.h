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

		std::shared_ptr<ClientConnect> CreateShortConnection(const std::string& key, const std::string& ip, int port);

		std::shared_ptr<ClientConnect> CreateLongConnection(const std::string& key, const std::string& ip, int port);

		std::shared_ptr<ClientConnect> CreateTemporaryConnection();

	private:
		std::shared_ptr<ClientConnect> CreateConnectionInternal(bool keepalive, const std::string& ip, int port);

	private:
		std::shared_mutex shared_mutex_;
		std::map<std::string, std::shared_ptr<ClientConnect>> connection_pool_;
	};
}