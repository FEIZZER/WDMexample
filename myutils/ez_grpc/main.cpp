#include "grpc_server/server.h"
#include "grpc_client/client_connect_write_stream.h"
#include "ez/log.hpp"

std::string Block()
{
	char input[1024];
	std::cin >> input;
	return std::string(input);
}

#include <sstream>
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
		LOG_INIT("ez", "C:\\Users\\Hillstone\\Desktop\\server.txt");
		{
			ez::server server;
			if (!server.start_server("127.0.0.1", 52000, grpc::InsecureServerCredentials()))
			{
				std::cout << "start server failed" << std::endl;
				return 1;
			}
			Block();
		}
	}
	else
	{
		LOG_INIT("ez", "C:\\Users\\Hillstone\\Desktop\\client.txt");
		ez::client_connect_write_stream connect(52000);
		
		std::string str;
		for (int i = 0; i < 100; i++)
		{
			if (Block() == "quit")
			{
				break;
			}
			str = std::to_string(i);
			if (!connect.request(str.c_str(), str.length()))
			{
				EZ_ERROR("request:{} failed", str);
			}
		}
		std::cout << "write done" << std::endl;

		connect.disconnect();

		// std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}
}