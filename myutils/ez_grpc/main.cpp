#include "grpc_server/server.h"
#include "grpc_client/client_connect_write_stream.h"
#include "ez/log.hpp"

int main(int args, char* argv[])
{
	LOG_INIT("ez", "C:\\Users\\Hillstone\\Desktop\\log.txt");

	if (args < 2)
	{
		std::cout << "wrong params" << std::endl;
		return 1;
	}

	char* param1 = argv[1];
	std::string strParam = std::string(param1);

	if (strParam == "1")
	{
		ez::server server;
		if (!server.start_server("127.0.0.1", 52000, grpc::InsecureServerCredentials()))
		{
			std::cout << "start server failed" << std::endl;
			return 1;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500000));
	}
	else
	{
		ez::client_connect_write_stream connect(52000);
		connect.request((void*)"haha", 4);
	}
}