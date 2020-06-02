#pragma once
#include<netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include"../bases/Logging.h"


namespace net
{
namespace socket
{

class InetAddr
{
public:
    InetAddr(unsigned int port, const char *ip = "")
    {
        bzero(&addr_, sizeof(addr_));
        addr_Init(ip,port);
    }
    InetAddr()
    {
        bzero(&addr_, sizeof(addr_));
    }

    ~InetAddr() = default;
    void addr_Init(const char *ip, unsigned int port);
    const struct sockaddr* ToSockAddr() const;               ///<sockaddr_in转sockaddr
    struct sockaddr* ToSockAddr() ;               ///<sockaddr_in转sockaddr
    socklen_t GetSockAddrLen() const
    {
        return sizeof(addr_);
    }
    std::string InetAddrToFormatString() const; ///<将地址格式标准输出为"IP:xxx,PORT:xxx"
    uint16_t GetPort() const;                   ///<获取端口的数字
    void GetIPStr(char *buf, int length) const;       ///<获取IP字符串:w


private:
    struct sockaddr_in addr_;

};

}   ///<end of namespace socket
}   ///<end of namespace net
