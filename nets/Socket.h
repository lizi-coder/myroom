#pragma once
#include"InetAddr.h"

namespace net
{
namespace socket
{

class Socket : public base::NonCopyable
{
public:
    Socket(int socket);
    ~Socket();

    int GetSocket() const
    {
        return socket_;
    }

    void Bind(InetAddr &targetAddr);    ///<作用同bind
    void ShutDown();

    int GetSockError();     ///<从getsockopt获取socket错误
    void SetNoDelay();      ///<设置禁用nagle

private:
    int socket_;
};

}   ///<END OF namespace socket
}   ///<END OF namespace net
