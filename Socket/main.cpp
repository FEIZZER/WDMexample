#include <iostream>
#include <string>

#include "grpcImp/client.h"
#include "grpcImp/server.h"
void test(void* request_buffer, int32_t request_length, void** reply_buffer, int32_t* reply_length)
{
    *reply_buffer = request_buffer;
    *reply_length = request_length;
}

int main(int argc, char** argv)
{
    #define V "testetst d"

    if (argc != 2)
    {
        return 1;
    }

    if (strcmpi(argv[1], "server") == 0)
    {
        std::cout << "server" << std::endl;
        ServerImp server(test);
        server.Run();

    }
    else if (strcmpi(argv[1], "stream_s") == 0)
    {

    }
    else if (strcmpi(argv[1], "client") == 0)
    {
        std::cout << "client" << std::endl;
        std::string target_str = "localhost:50051";
        ClientImpl base(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
        std::string reply = base.ClientStreamTransmit(V, sizeof(V));
    }
    else if (strcmpi(argv[1], "async_c") == 0)
    {
        std::cout << "async_c" << std::endl;
        std::string target_str = "localhost:50051";
        ClientImpl base(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
        
        base.LoopReceiveResponse();

        for (size_t i = 0; i < 10; i++)
        {
            base.AsyncTransmit(V, sizeof(V));
        }
        Sleep(7000);
    }
    else
    {
        return 1;
    }
	return 0;
}


//int StartClientSocket()
//{
//	try
//	{
//		const char* ipaddr = "127.0.0.1";
//		Poco::Net::SocketAddress sa(ipaddr, 12345);
//		Poco::Net::StreamSocket dgs;
//		dgs.connect(sa);
//
//		std::string syslogMsg;
//		Poco::Timestamp now;
//		syslogMsg = Poco::DateTimeFormatter::format(now, "<14>%w %f %H:%M:%S Hello,world!");
//		dgs.sendBytes(syslogMsg.data(), syslogMsg.size());
//		return true;
//	}
//	catch (...)
//	{
//		return false;
//	}
//
//}
//bool StartServerSocket()
//{
//	Poco::Net::ServerSocket s_socket(12345);
//	while (true)
//	{
//		Poco::Net::StreamSocket streamSocket = s_socket.acceptConnection();
//		Poco::Buffer<char> bufTemp(0);
//		int nRecv = streamSocket.receiveBytes(bufTemp);
//		std::string str(bufTemp.begin(), nRecv);
//		std::cout << "request: " << str << std::endl;
//
//		Poco::Net::SocketStream socketStream(streamSocket);
//
//		socketStream << "HTTP/1.0 200 OK\r\n"
//			<< "Content-Type: text/html\r\n"
//			<< "\r\n"
//			<< "<html><head><title>My 1st Web Server</title></head></html>"
//			<< std::flush;
//	}
//	return true;
//}