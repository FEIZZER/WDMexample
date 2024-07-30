#include "grpc_server/server.h"
#include "ez_grpc/client/client.h"
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

	
	ez_grpc::BasicServer server;
	if (cmdline == "server")
	{

		if (!server.BuildServer(8800))
		{
			std::cout << "build server failed";
		}

	}
	else if (cmdline == "client")
	{
		ez_grpc::Client client;
		client.CreateShortConnection("test_connect", "127.0.0.1:", 8800)->Request((void*)"hahaha", 6);
	}
	else
	{
		std::cout << "error cmdline" << std::endl;
	}

	Block();

	return 0;
}