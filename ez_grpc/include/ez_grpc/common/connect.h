#pragma once
#include <string>

namespace ez_grpc {

	class ConnectInterface
	{
	public:
		ConnectInterface() = default;
		virtual ~ConnectInterface() = default;

	protected:
		bool connected_;

		std::string ip_;

		int client_port_;
		int server_port_;

	};

}