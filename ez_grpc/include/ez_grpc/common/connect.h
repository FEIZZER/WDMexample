#pragma once
#include <string>

namespace ez_grpc {

	class Connect
	{
	public:
		Connect() = default;
		virtual ~Connect() = default;

	protected:
		bool connected_;

		std::string ip_;

		int client_port_;
		int server_port_;

	};

}