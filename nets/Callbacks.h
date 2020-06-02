#pragma once
#include<functional>
#include<memory>
#include"../bases/TimeStamp.h"
#include"Buffer.h"

namespace net
{
class TcpConnection;

    typedef std::function<void()> ChannelCallBack;
    typedef std::function<void()> IOThreadCallBack; ///< for EVENTloop线程用户回调
    typedef std::function<void()> TimerFunc;    ///<for 定时器
    typedef std::function<void(int newConnFd)> AcceptorCallBack;    ///<for Accept后调用的回调函数，其中newConnFd是新建立好的socketfd

    typedef std::shared_ptr<TcpConnection>  TcpConnectionPtr;

    //服务器,即用户传给TCPServer的函数
    typedef std::function<void(const TcpConnectionPtr &ptr, Buffer &buffer, base::TimeStamp &recvTime)> TcpServerReadCallBack;
    typedef std::function<void(const TcpConnectionPtr &ptr, Buffer &buffer, base::TimeStamp &recvTime)> TcpServerWriteCallBack;
    typedef std::function<void(const TcpConnectionPtr &ptr)> TcpServerWriteCompeleteCallBack;
    typedef std::function<void(const TcpConnectionPtr &)> OnConnectionCallback;


    //以下四个为内部使用
    //TcpServer或者TCPClient传给TCPConnection的参数(即TCPConnection会去调用这些回调函数)
    typedef std::function<void(const TcpConnectionPtr &ptr, Buffer& buffer, base::TimeStamp &recvTime)> TcpConnectionReadCallBack;    ///<读回调，当有数据来时，网络库先读数据，再调用这个
    typedef std::function<void(const TcpConnectionPtr &, Buffer& buffer, base::TimeStamp &recvTime)> TcpConnectionWriteCallBack;           ///<当可写时触发用户写回调（可写指的是TCP发送缓冲区里面有空位时）
    typedef std::function<void(const TcpConnectionPtr &)> TcpConnectionWriteCompeleteCallBack;      ///<写完毕时触发完成写回调，（完毕指的是用户指定的数据已经全部放入TCP缓冲区)

    typedef std::function<void(const TcpConnectionPtr &)> OnCloseCallback;      ///<仅用于TCPConnection调用TCPServer的清除函数或用于TCPClient中

    //客户端
    typedef std::function<void(const TcpConnectionPtr &ptr, Buffer &buffer, base::TimeStamp &recvTime)> TcpClientReadCallBack;
    typedef std::function<void(const TcpConnectionPtr &ptr, Buffer &buffer, base::TimeStamp &recvTime)> TcpClientWriteCallBack;
    typedef std::function<void(const TcpConnectionPtr &ptr)> TcpClientWriteCompeleteCallBack;
    typedef std::function<void(const TcpConnectionPtr &)> OnConnectionCallback;

    //Only For Connector
    typedef std::function<void(int sockfd)>   ConnectorConnectOKCb;
    typedef std::function<void()>   ConnectorConnectFailCb;     



    

}


