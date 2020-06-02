#include <asm-generic/socket.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <unistd.h>
#include"../bases/Logging.h"
#include"Socket.h"
#include"InetAddr.h"
using namespace base;
namespace net
{
namespace socket
{

Socket::Socket(int socket)
    :socket_(socket)
{
    if(socket_ < 0)
    {
        LOG_FATAL<<"SOCKET InValid";
    }
}
Socket::~Socket()
{
    close(socket_);
}

void Socket::Bind(InetAddr &targetAddr)
{
    int isOK = ::bind(socket_, targetAddr.ToSockAddr(),targetAddr.GetSockAddrLen());
    if(isOK)
    {
        LOG_FATAL<<"Bind Error!";
    }

}
void Socket::ShutDown()
{
    ::shutdown(socket_, SHUT_WR);
}

int Socket::GetSockError()
{
    int res     = 0;
    socklen_t reslen  = sizeof (int);
    int n = getsockopt(socket_, SOL_SOCKET, SO_ERROR, &res, &reslen );
    assert(n == 0 );
    return res;
}

void Socket::SetNoDelay()
{
    int res     = 0;
    socklen_t reslen  = sizeof (int);
    //int n = setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, &res, &reslen );    ///<这个会报错，后面再看看
    //assert(n == 0 );
}

}   ///<END OF namespace socket
}   ///<END OF namespace net
