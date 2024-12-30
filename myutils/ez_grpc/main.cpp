#include "grpc_server/server.h"
#include "grpc_client/client_connect_write_stream.h"


int main(int args, char* argv[])
{
	if (args < 2)
	{
		std::cout << "wrong params" << std::endl;
		return 1;
	}

	char* param1 = argv[1];
	std::string strParam = std::string(param1);

	if (strParam == "1")
	{
		ez_grpc::server server;
		if (!server.start_server("0.0.0.0", 50051, grpc::InsecureServerCredentials()))
		{
			std::cout << "start server failed" << std::endl;
			return 1;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500000));
	}
	else
	{
		ez_grpc::client_connect_write_stream connect(50051);
		connect.request((void*)"haha", 4);
	}
}