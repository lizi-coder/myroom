#pragma once
#include"Connector.h"
#include"Callbacks.h"
#include "EventLoop.h"
#include"TcpConnection.h"
#include <memory>


namespace net
{

class TcpClient
{
public:
    TcpClient(EventLoop *loop, const char* serverIP, unsigned int serverPort);
    ~TcpClient() {}

      
    void SetReadCallback(TcpClientReadCallBack cb)                     {  readCallback_ = std::move(cb);}
    void SetWriteCallback(TcpClientWriteCallBack cb)                   {writeCallback_ = std::move(cb); }
    void SetWriteCompleteCallback(TcpClientWriteCompeleteCallBack cb)  {writeCompeleteCallbck_ = std::move(cb);  }
    void SetOnConnectCallback(OnConnectionCallback cb)                 { onConnectCb_ = std::move(cb); }

    void Quit();
    void Start();
    void NewConnection(int sockFd);
private:
    void ConfirmConnect(const TcpConnectionPtr &ptr, Buffer &buf, base::TimeStamp &recvTime);    ///<用于第一次监听其可写时调用（连接确认功能）
    void ConnectOver(const TcpConnectionPtr &ptr);
private:

    EventLoop *loop_;
    std::unique_ptr<socket::InetAddr> serverAddr;
    std::unique_ptr<Connector>  connector_;
    std::unique_ptr<Channel>    clientChannel_;    
    TcpConnectionPtr            tcpConnetion_;
    int sockFd;

    
    //用户回调
    OnConnectionCallback    onConnectCb_;
    TcpClientReadCallBack   readCallback_;
    TcpClientWriteCallBack  writeCallback_;
    TcpClientWriteCompeleteCallBack writeCompeleteCallbck_;

    

};

}
