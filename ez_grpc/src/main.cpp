#include "grpc_server/server.h"
#include "grpc_server/stream_server.h"
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

ez_grpc::ez_request_handler test_handler = [](void* buffer, int length, void** out_buffer, int* out_length)
{
	std::string strBuf((char*)buffer, length);
	std::cout << strBuf;

	*out_buffer = new char[10];
	*out_length = 10;

	memcpy(*out_buffer, "1234567890", 10);
};


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


	
	ez_grpc::Server server;
	ez_grpc::StreamServer stream_server;
	ez_grpc::Client client;
	if (cmdline == "server")
	{

		/*if (!server.BuildServer(8800, test_handler))
		{
			std::cout << "build server failed";
		}*/

		if (!stream_server.Init())
		{
			std::cout << "build server failed" << std::endl;
		}

	}
	else if (cmdline == "client")
	{
		void* reply_buffer = nullptr;
		unsigned int reply_length = 0;
		// client.CreateShortConnection("test_connect", "127.0.0.1:", 8800)->Request((void*)"hahaha", 6, &reply_buffer, &reply_length);
		if (!client.CreateLongConnection("test_connect", "127.0.0.1:", 8800)->Request((void*)"hahaha", 6))
		{
			std::cout << "request failed" << std::endl;
		}
		std::string strReply((char*)reply_buffer, reply_length);
		std::cout << "reply: " << strReply << std::endl;
	}
	else
	{
		std::cout << "error cmdline" << std::endl;
	}

	Block();

	return 0;
}