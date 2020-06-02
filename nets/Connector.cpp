#include"Connector.h"
#include "Channel.h"
#include"SocketOpts.h"
#include"EventLoop.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cstddef>
#include <sys/poll.h>
#include <sys/socket.h>

using namespace base;
using namespace net::socket;
namespace net
{

Connector::Connector(EventLoop *loop, socket::InetAddr &peerAddr)
        :loop_(loop),
         sockfd(-1),
         peerAddr_(peerAddr),
         connectRound_(0),
         isReConnecting_(false),
         curState_(kBegin)
    {

    }

void Connector::Start()
{
    loop_->RunInLoop(std::bind(&Connector::Connect, this));
}
int  Connector::GetSockFd()
{
    return (curState_ != kConnected) ? -1 : sockfd;
}

void Connector:: Connect()      ///<仅限在定时器回调中使用
{
    assert(curState_ != kConnecting);
    SetState(kConnecting);

    sockfd = CreateSocket();
    int connectRes = ::connect(sockfd, peerAddr_.ToSockAddr(), peerAddr_.GetSockAddrLen());



    ///<注意后面创建Chanel时还要监听其是否可写！(连接刚到来时，其既可读，又可写)
    switch (errno)   
  {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
      Connecting(sockfd);
      break;
    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
      Retry(sockfd);
      break;
    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
      LOG_ERROR << "connect error in Connector::startInLoop ";
      ::close(sockfd);
      loop_->Quit();
      break;

    default:
      LOG_ERROR << "Unexpected error in Connector::startInLoop ";
      loop_->Quit();
      ::close(sockfd);
      // connectErrorCallback_();
      break;
  }
}


void Connector::Retry(int sockfd)
{
    ::close(sockfd);
    SetState(kRetry);            
    LOG_ERROR<<"Connect Error!";
    ReConnectSetTimer();

}

void Connector::Connecting(int sockfd)
{   
     SetState(kConnectingOK);                                                                                                                 
     struct pollfd connectFd;                                                                                                                 
     connectFd.events = POLLOUT | POLLIN;                                                                                                     
     connectFd.fd     = sockfd;                                                                                                               
     connectFd.revents = 0;                                                                                                                
     int n = ::poll(&connectFd, 1, 0);                                                                                                     

     if((connectFd.revents & POLLOUT && connectFd.revents & POLLIN))    ///<UNP351页，非阻塞connect当遇到错误时变为即可读又可写            
         Retry(sockfd);                                                                                                                    
     else if((connectFd.revents & POLLOUT) && !(connectFd.revents & POLLIN))                                                               
     {                                                                                                                                     
         LOG_TRACE<<"Connect ok!";                                                                                                         
         assert(okCb_);                                                                                                                    
         SetState(kConnectingOK);                                                                                                          
         okCb_(sockfd);                                                                                                                    
     }                                                                                                                                     
     else                                                                                                                                  
     {                                                                                                                                     
         LOG_ERROR<<"Unknown revent Type for connection socket";                                                                           
     }                                                                                                                                     
}                                                                                                                                                

void Connector::ReConnect()           ///<定时器回调函数
{
    assert(isReConnecting_ == true);
    assert(curState_  == kRetry);
    isReConnecting_  = false;
    Connect();
}


void Connector::ReConnectSetTimer()                                                                                                  
{
  connectRound_ ++;                                                                                    
  if(connectRound_ > kRetryTimes)     ///<尝试重连次数超过规定                                          
  {                                                                                                    
      SetState(kFail);                                                                                 
      if(failCb_)                                                                                      
          failCb_();                                                                                   
      LOG_FATAL<<"Connect fail...";                                                                    
      return;                                                                                          
  }                                                                                                    
  LOG_INFO<<"Now Try ReConnect...times "<<connectRound_;                                               
  loop_->RunAfter(loop_, std::bind(&Connector::ReConnect, this), reconnectTime[connectRound_] );       
}


}
