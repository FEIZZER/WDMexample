#pragma once
#include <string>
#include <memory>

#include "service.h"

namespace ez {

class server
{
public:
	server();
	~server();

	bool start_server(const std::string& ip, unsigned int port, std::shared_ptr<ServerCredentials>);

private:
	std::unique_ptr<grpc::Server> grpc_server_;
	client_stream_service client_stream_service_;
};


}
