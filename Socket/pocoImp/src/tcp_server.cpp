#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include <iostream>

bool StartServerSocket1()
{
	Poco::Net::ServerSocket s_socket(12345);
	while (true)
	{
		std::cout << "1" << std::endl;
		Poco::Net::StreamSocket streamSocket = s_socket.acceptConnection();
		std::cout << "2" << std::endl;
		Poco::Buffer<char> bufTemp(0);
		int nRecv = streamSocket.receiveBytes(bufTemp);
		std::string str(bufTemp.begin(), nRecv);
		std::cout << "res:" << str << std::endl;

		Poco::Net::SocketStream socketStream(streamSocket);

		socketStream << "HTTP/1.0 200 OK\r\n"
			<< "Content-Type: text/html\r\n"
			<< "\r\n"
			<< "<html><head><title>My 1st Web Server</title></head></html>"
			<< std::flush;
	}
	return true;
}