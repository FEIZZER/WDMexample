#include "grpc_server/server.h"
#include "grpc_client/client.h"
#include <iostream>

void Block()
{
	char input[1024] = { 0 };
	std::string quit = "quit";
	std::cout << "enter \"" << quit << "\" continue" << std::endl;

	do {
		
		std::cin >> input;

		if (quit == input)
		{
			break;
		}

	} while (true);
}


int main(int args, char* argv[])
{

	std::string cmdline;
	if (args <= 1)
	{
		cmdline = "server";
	}
	else
	{
		cmdline = argv[1];
	}

	
	grpc_cpp_test::BasicServer server;
	if (cmdline == "server")
	{

		if (!server.BuildServer(8800))
		{
			std::cout << "build server failed";
		}

	}
	else if (cmdline == "client")
	{
		grpc_cpp_test::Client client;

		client.Request(8800, "ok", 2);
	}
	else
	{
		std::cout << "error cmdline" << std::endl;
	}

	Block();

	return 0;
}