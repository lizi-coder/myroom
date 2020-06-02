#include<assert.h>
#include <algorithm>
#include <cstdio>
#include <exception>
#include <functional>
#include <unistd.h>
#include"Channel.h"
#include"EventLoop.h"
#include"TcpConnection.h"
#include"Socket.h"
#include "Callbacks.h"
#include"Buffer.h"
#include"../bases/TimeStamp.h"

using namespace base;
using namespace net::socket;
namespace net
{

TcpConnection::TcpConnection(EventLoop *loop, int newConnFd, socket::InetAddr peerAddr, socket::InetAddr localAddr)
                :loop_(loop),
                 socket_(new Socket(newConnFd)),
                 connectionChannel_( new Channel(loop_, newConnFd)),
                 peerAddr_(std::move(peerAddr)),
                 localAddr_(std::move(localAddr)),
                 inputBuffer_(),
                 outputBuffer_(),
                 connectTime_(TimeStamp::now()),
                 state_(kConnecting),
                 isWriting(false)

{
    LOG_DEBUG <<"new TcpConnection Create, SocketNum is "<<socket_->GetSocket();
    //注意此处的工作流程：当套接字可读时，Chanel会先调用TcpConnection::HandleRead，由handleRead读完数据后再调用用户回调
    connectionChannel_->SetReadCallBack(std::bind(&TcpConnection::HandleRead, this));
    connectionChannel_->SetWriteCallBack(std::bind(&TcpConnection::HandleWrite, this));
    connectionChannel_->SetErrorCallBack(std::bind(&TcpConnection::HandleError, this));
}
TcpConnection::~TcpConnection()
{
    LOG_TRACE<<"TcpConnection dtor!!, FD = " <<socket_->GetSocket();
}


void TcpConnection::Send(const char* msg, int len)
{
    loop_->RunInLoop(std::bind(&TcpConnection::SendInLoop, this, msg, len));

}
void TcpConnection::Send(const std::string& msg)
{
    loop_->RunInLoop(std::bind(&TcpConnection::SendInLoop, this, msg.data(), msg.size() ));

}
void TcpConnection::Send(Buffer& buffer)
{

}
void TcpConnection::Send(const void *msg, int len)
{
    loop_->RunInLoop(std::bind(&TcpConnection::SendInLoop, this, static_cast<const char*>(msg), len));
}
void TcpConnection::SendInLoop(const char* msg, int length)       ///<思考为什么需要SendInLoop;
{
    if(state_ == kConnected)
    {
        ssize_t n = ::write(socket_->GetSocket(), msg, length);
        if( n >= 0 )
        {
            if(n == length)
            {
                LOG_TRACE<<"socket "<<socket_->GetSocket()<<" Write data full Sccuess!";
            }
            else if (n < length)        ///<如果一次写不完就要把剩下的数据放到outputbuffer中并去监听POLLOUT
            {
                isWriting = true;
                connectionChannel_->EnableWriting();
                LOG_TRACE<<"socket "<<socket_->GetSocket()<<" wants wirte more data...lack "<<static_cast<int>(length - n);
            }
        }
        else
        {
            LOG_DEBUG<<"socket "<<socket_->GetSocket()<<"write error occur!";
        }
    }

}
void TcpConnection::Shutdown()                                    ///<主动关闭连接，注意转移！
{
    if(state_ == kConnected)
    {
        SetState(kDisConnecting);
        loop_->RunInLoop(std::bind(&TcpConnection::ShutdownInLoop, this));
    }

}
void TcpConnection::ShutdownInLoop()                              ///<思考：为什么需要在Loop里shutdown呢？P317
{
    //思考：为什么需要判断Channel正在写呢？ 
    if(isWriting == false)
    {
        socket_->ShutDown();
    }

}

void TcpConnection::HandleRead()
{
    size_t n = inputBuffer_.ReadFd(socket_->GetSocket());
    LOG_TRACE<<"HandleRead calls! fd = "<<socket_->GetSocket()<<" read "<<n<<" bytes";
    //逻辑：先读东西到缓冲区，如果读=0则调用handleClose,如果出错则调用handleError
    if(n > 0)
    {   
        LOG_TRACE<<"socket:"<<socket_->GetSocket()<<" has recv "<<n<<" bytes";
        TimeStamp recvTime = TimeStamp::now();
        if(readCallback_)
            readCallback_(shared_from_this(), inputBuffer_, recvTime);
    }
    else if (n == 0)
        HandleClose();
    else
        HandleError();
}
void TcpConnection::HandleWrite()   ///<目标：把outputBuffer里的所有数据全部发出去,如果发完则关POLLOUT，否则还打开
{
    TimeStamp now = TimeStamp::now();
    size_t left = outputBuffer_.DataNumber();
    if(left > 0)
    {
        size_t n = ::write(socket_->GetSocket(), outputBuffer_.Peek(), left);
        if(n == left)
        {
            connectionChannel_->DisableWriting();
            outputBuffer_.RetrieveAll();
            if(state_ == kDisConnecting)
                socket_->ShutDown();
        }
        else if(n > 0)
        {
            outputBuffer_.Retrieve(n);
        }
        else
        {
            LOG_ERROR<<"Write Socket:"<<socket_->GetSocket()<<" error occur!";
        }
    }
    LOG_TRACE<<"HandleWrite calls";
    if(writeCallback_)
        writeCallback_(shared_from_this(),outputBuffer_, now);
}
void TcpConnection::HandleError()
{
    LOG_ERROR<<"TcpConnection Error happens";
    HandleClose();
}
void TcpConnection::HandleClose()
{
    LOG_DEBUG<<"FD "<<socket_->GetSocket()<<" into handleclose...now state is " <<state_;
    if(state_ == kConnected || state_ == kDisConnecting)
    {
        SetState(kDisConnected);
        connectionChannel_->DisableAll();
        LOG_TRACE<<"Now TcpConnection "<<socket_->GetSocket()<<" close...";

        TcpConnectionPtr ptr(shared_from_this());

        if(onConnectionCallback_)
            onConnectionCallback_(ptr);         ///<当连接关闭时调用用户的连接回调
        LOG_DEBUG<<"Call user CallBack over!, now run TCPServer RemoveConnection...";
         LOG_TRACE<<"In Function:" <<__func__<<" shared_ptr<TcpConnection>.use_count is "<<static_cast<int>(ptr.use_count());
        onCloseCallback_(ptr);
    }
}
void TcpConnection::ConnectEstablished()      ///<连接建立后，由TCPServer中的NewConnection调用此函数
{
    //BUG:这个动作必须要在子线程的IO循环里做！！之前是直接在这里写函数体，发生了多个fd都往监听线程的vector去写Channel!
    loop_->RunInLoop(std::bind(&TcpConnection::ConnectEstablishedInLoop, this));
}

void TcpConnection::ConnectEstablishedInLoop()
{
    if(state_  == kConnecting)
    {
        SetState(kConnected);
        LOG_TRACE<<"In Function:" <<__func__<<" State change into kConnected ";
        connectionChannel_->EnableReading();
        connectionChannel_->SetTiedPtr(shared_from_this());
        if(onConnectionCallback_)
            onConnectionCallback_(shared_from_this());
    }

}

void TcpConnection::ConnectDestroy(const TcpConnectionPtr &ptr)
{
    assert( state_ == kDisConnected );
    LOG_TRACE<<"TcpConnection Destroying...";
    assert(connectionChannel_ != NULL);
    connectionChannel_->RemoveChannel();
}


void TcpConnection::StopTimer(TimerID *TimerID)
{
    loop_->RunInLoop(std::bind(&EventLoop::CancelTimer, loop_, TimerID));
}
}
