#include "Poco/Net/DatagramSocket.h"  
#include "Poco/Net/SocketAddress.h"  
#include "Poco/Timestamp.h"  
#include "Poco/DateTimeFormatter.h" 
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Event.h"
#include <iostream>
#include <string>

int StartClientSocket()
{
	try
	{
		const char* ipaddr = "127.0.0.1";
		Poco::Net::SocketAddress sa(ipaddr, 12345);
		Poco::Net::StreamSocket dgs;
		dgs.connect(sa);

		std::string syslogMsg;
		Poco::Timestamp now;
		syslogMsg = Poco::DateTimeFormatter::format(now, "<14>%w %f %H:%M:%S Hello,world!");
		dgs.sendBytes(syslogMsg.data(), syslogMsg.size());
		return true;
	}
	catch (...) 
	{
		return false;
	}

}

bool StartServerSocket()
{
	Poco::Net::ServerSocket s_socket(12345);
	while (true)
	{
		Poco::Net::StreamSocket streamSocket = s_socket.acceptConnection();
		Poco::Buffer<char> bufTemp(0);
		int nRecv = streamSocket.receiveBytes(bufTemp);
		std::string str(bufTemp.begin(), nRecv);
		std::cout << "request: " << str << std::endl;

		Poco::Net::SocketStream socketStream(streamSocket);

		socketStream << "HTTP/1.0 200 OK\r\n"
			<< "Content-Type: text/html\r\n"
			<< "\r\n"
			<< "<html><head><title>My 1st Web Server</title></head></html>"
			<< std::flush;
	}
	return true;
}

int main(int argc, char** argv)
{
	
	Poco::Event event(false);

	if (event.tryWait(2000)) 
	{
		std::cout << "1";
	}

	std::cout << "2";




	return 0;
}