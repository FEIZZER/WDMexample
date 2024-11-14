#pragma once
#include "server_connect.h"

#include <string>

namespace ez_grpc {

	class server
	{
	public:
		server();
		virtual ~server();

		bool Start(ez_event_handler handler, const std::string& ip, const std::string& port);

	private:
		int local_port_;
	};
}