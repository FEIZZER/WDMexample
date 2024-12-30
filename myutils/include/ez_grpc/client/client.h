#pragma once

#include <mutex>
#include <shared_mutex>
#include <memory>
#include <map>

#include "ez_grpc/connect.h"
#include "ez_grpc/client/client_connect.h"

namespace ez_grpc {

	class Client
	{
	public:
		Client();
		~Client();

		std::shared_ptr<client_connect> CreateShortConnection(const std::string& key, const std::string& ip, int port);

		std::shared_ptr<client_connect> CreateLongConnection(const std::string& key, const std::string& ip, int port);

		std::shared_ptr<client_connect> CreateTemporaryConnection();

	private:
		std::shared_ptr<client_connect> CreateConnectionInternal(bool keepalive, const std::string& ip, int port);

	private:
		std::shared_mutex shared_mutex_;
		std::map<std::string, std::shared_ptr<client_connect>> connection_pool_;
	};
}