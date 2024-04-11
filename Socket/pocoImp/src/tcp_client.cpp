#include "Poco/Net/DatagramSocket.h"  
#include "Poco/Net/SocketAddress.h"  
#include "Poco/Timestamp.h"  
#include "Poco/DateTimeFormatter.h"  
#include <string>

int client()
{
    const char* ipaddr = "127.0.0.1";
    Poco::Net::SocketAddress sa("127.0.0.1", 12345);
    Poco::Net::DatagramSocket dgs;
    dgs.connect(sa);

    std::string syslogMsg;
    Poco::Timestamp now;
    syslogMsg = Poco::DateTimeFormatter::format(now, "<14>%w %f %H:%M:%S Hello,world!");
    dgs.sendBytes(syslogMsg.data(), syslogMsg.size());

    return 0;
}