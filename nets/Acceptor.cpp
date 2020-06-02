

#include"Acceptor.h"
#include "Callbacks.h"
#include "EventLoop.h"
#include"../bases/Logging.h"
#include "Socket.h"
#include"SocketOpts.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <asm-generic/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace base;

namespace net
{

Acceptor::Acceptor(EventLoop *loop, const char* IPAddr, int port)
            :loop_(loop),
             listenFd_(socket::CreateSocket()),
             serverAddr_(port, IPAddr),
             clientAddr_(),
             listenChannel_(new Channel(loop_, listenFd_.GetSocket())),
             readCallBack_(),
             foolishFd(::open("/dev/null", O_RDONLY))
{
    int i = 1;
    setsockopt(listenFd_.GetSocket(), SOL_SOCKET, SO_REUSEPORT, &i, sizeof(i));
    assert(IPAddr != NULL);
    assert(port < 65536);
    assert(foolishFd >= 0);

    listenChannel_->SetReadCallBack(std::bind(&Acceptor::AcceptAndCallback, this));
    listenChannel_->EnableReading();

    if(foolishFd < 0)
    {
        LOG_FATAL<<"foolishFd open Error!";
    }

    listenFd_.Bind(serverAddr_);
    Listen();
    
}

void Acceptor::SetReadCallBack(AcceptorCallBack cb)
{
    readCallBack_ = std::move(cb);
}

void Acceptor::AcceptAndCallback()      ///<连接并调用其上层的读回调
{
    int newConnFd = Accept();
    if(newConnFd > 0)
    {
        readCallBack_(newConnFd);
    }
    else
    {
        LOG_ERROR<<"Accept Error!";
    }
}

//accept策略：留一个闲余fd，当连接满了（fd满），那就关闭闲鱼fd再accept，accept后立马关闭newConnfd再开启闲鱼fd
int Acceptor::Accept()  ///<问题：在Acceptor里怎么处理accept返回的sockaddr呢？
{
    socklen_t length = clientAddr_.GetSockAddrLen();
    errno = 0;
    int newConnFd = ::accept(listenFd_.GetSocket(), clientAddr_.ToSockAddr(), &length);
    assert(errno != EINVAL);
    assert(errno != EWOULDBLOCK && errno != EAGAIN);    ///<不可能出现这两个，因为是listenfd可读才会运行到此处
    if(errno == EMFILE || errno == ENFILE)  ///<fd超限
    {
        close(foolishFd);
        newConnFd = ::accept(listenFd_.GetSocket(), clientAddr_.ToSockAddr(), &length);
        close(newConnFd);
        newConnFd = -1;
        foolishFd = ::open("/dev/null", O_RDWR);
    }
    return newConnFd > -1 ? newConnFd : -1;
}


void Acceptor::Listen()
{
    int isOK = ::listen(listenFd_.GetSocket(), kDefaultListenValue);
    if(isOK)
    {
        LOG_FATAL<<"Listen fail!";
    }
}
    int Acceptor::kDefaultListenValue = 1000;

}   ///< end of net
