#include"../bases/Logging.h"
#include"TcpConnection.h"
#include "Callbacks.h"
#include "Channel.h"
#include"InetAddr.h"
#include"TcpClient.h"
#include"EventLoop.h"
#include <functional>
#include <netinet/in.h>
#include <sys/socket.h>
using namespace base;

using namespace std::placeholders;
using namespace net::socket;
namespace net
{


TcpClient::TcpClient(EventLoop *loop, const char* serverIP, unsigned int serverPort)
                :loop_(loop),
                 serverAddr(new socket::InetAddr(serverPort, serverIP)),
                 connector_(new Connector(loop_,*serverAddr)),
                 clientChannel_(),
                 tcpConnetion_(),
                 sockFd(-1)
{
    LOG_INFO<<"After loop, Clinet Connect "<<serverIP<<"...";
    connector_->SetConnectOkCallBack(std::bind(&TcpClient::NewConnection, this, std::placeholders::_1));
}

void TcpClient::Start()
{
    connector_->Start();
}
void TcpClient::Quit()
{
    LOG_INFO<<"Client Quit Loop!";
    loop_->Quit();
}
void TcpClient::NewConnection(int sockFd)   ///<sockFd：已成功连接的套接字
{
    //当连接成功时，会调用此函数！
    assert(connector_->GetCurState() == Connector::kConnectingOK);
    //获取本地地址及端口，即TCPClient端
    InetAddr localAddr;
    socklen_t length = localAddr.GetSockAddrLen();
    getsockname(sockFd, localAddr.ToSockAddr(), &length);
    //建立TcpConnetion
    LOG_DEBUG<<"in "<<__func__<<",begin create TcpConnection";
    tcpConnetion_.reset(new TcpConnection(loop_, sockFd, *serverAddr, localAddr));


    tcpConnetion_->SetReadCallback(readCallback_);
    tcpConnetion_->SetOnConnectionCallback(onConnectCb_);
    tcpConnetion_->SetWriteCompleteCallback(writeCompeleteCallbck_);
    tcpConnetion_->SetWriteCallback(writeCallback_);

    tcpConnetion_->SetCloseCallback(std::bind(&TcpClient::ConnectOver, this, std::placeholders::_1));
    //注意这边第一次注册的并不是用户的writecb，而是连接确认函数
    //tcpConnetion_->SetWriteCallback(std::bind(&TcpClient::ConfirmConnect, this, _1, _2, _3));
    //监听其可写事件，如果（第一次时）可写说明真正连接成功了
    tcpConnetion_->SetState(TcpConnection::kConnecting);
    tcpConnetion_->ConnectEstablished();
}

void TcpClient::ConfirmConnect(const TcpConnectionPtr &ptr, Buffer &buf, TimeStamp &recvTime)    ///<用于第一次监听其可写时调用（连接确认功能）
{
    LOG_INFO<<"Connect Scuess!";
    tcpConnetion_->SetWriteCallback(writeCallback_);
    
}
void TcpClient::ConnectOver(const TcpConnectionPtr &ptr)
{
    LOG_INFO<<"Connect finish";
    Quit();
}


}
