
#include<bits/stdint-uintn.h>
#include<netinet/in.h>
#include<assert.h>
#include<string>
#include<strings.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include"InetAddr.h"

using namespace base;
namespace net
{
namespace socket
{

void InetAddr::addr_Init(const char *ip, unsigned int port)
{
    assert(ip != NULL && port < 65532);
    addr_.sin_port = ::htons(port);
    addr_.sin_family = AF_INET;
    if(strlen(ip) == 0)
    {
        addr_.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
       int n = inet_pton(AF_INET, ip, (void*)&addr_.sin_addr.s_addr);
       if(n != 1)
       {
           LOG_FATAL<<"IP ERROR";
       }
    }

}
struct sockaddr* InetAddr::ToSockAddr()
{
    return reinterpret_cast<struct sockaddr*>(&addr_);
}

const struct sockaddr* InetAddr::ToSockAddr() const
{
    return (const struct sockaddr*)(&addr_);
}

std::string InetAddr::InetAddrToFormatString() const
{
    std::string strAddr = "IP:";
    char ipBuf[ 64 ] = { 0 };

    inet_ntop(AF_INET, (void*)&addr_.sin_addr.s_addr, ipBuf, sizeof ipBuf);

    strAddr = strAddr + ipBuf + ",PORT:" + std::to_string(::ntohs(addr_.sin_port)); 
    return strAddr;
}

uint16_t InetAddr::GetPort() const
{
    return ::ntohs(addr_.sin_port);
}
void InetAddr::GetIPStr(char *buf, int length) const
{
    const char *ip = inet_ntop(AF_INET, (void*)&addr_.sin_addr.s_addr, buf, length);
    assert(ip != NULL);
}

}   ///<end of namespace socket
}   ///<end of namespace net
