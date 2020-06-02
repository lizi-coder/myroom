#pragma once
#include <algorithm>
#include <memory>
#include"../bases/NonCopyable.h"
#include"../bases/TimeStamp.h"
#include"Callbacks.h"
#include"Buffer.h"
#include"InetAddr.h"
#include"Socket.h"
#include "TimerID.h"

/**
 * 作用：对单次连接进行抽象，在单次连接上的所有用户交互均通过此类进行
 */

namespace net
{
class EventLoop;
class Channel;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>, base::NonCopyable
{
public:
    enum StateE { kConnecting, kConnected, kDisConnecting, kDisConnected };     ///<TCP连接四状态：连接中（创建TCPConnection时）

                                                                                ///<               连接后（调用TCPConnection::ConnectEstablished后）
                                                                                //                 连接结束中（调用Shutdown(用户主动关闭)）
                                                                                //                 结束后（调用ConnectionDestroy）
    TcpConnection(EventLoop *loop, int newConnFd, socket::InetAddr peerAddr, socket::InetAddr localAddr);
    ~TcpConnection();

    void Send(const char* msg, int len);
    void Send(const std::string&);
    void Send(Buffer& buffer);
    void Send(const void *msg, int len);
    void SendInLoop(const char* msg, int length);       ///<思考为什么需要SendInLoop;
    void Shutdown();                                    ///<主动关闭连接，注意转移！
    void ShutdownInLoop();                              ///<思考：为什么需要在Loop里shutdown呢？P317

    int GetSocketFd() const  { return socket_->GetSocket(); }
    EventLoop* GetLoop() const { return loop_; }
    bool Connected()   const  { return state_ == kConnected; }          ///<判断当前是处于连接状态还是连接关闭状态
    bool Disconnected()   const  { return state_ == kDisConnected; }
    

    //由TCPClient或者TCPSever调用
    void SetOnConnectionCallback(OnConnectionCallback &cb)   { onConnectionCallback_ = cb; }
    void SetReadCallback(TcpConnectionReadCallBack &cb)      { readCallback_ = cb; }
    void SetWriteCallback(TcpConnectionWriteCallBack &cb)    { writeCallback_ = cb; }
    void SetWriteCompleteCallback(TcpConnectionWriteCompeleteCallBack &cb)   { writeCompCallback_ = cb;}
    void SetCloseCallback(OnCloseCallback cb)       { onCloseCallback_ = std::move(cb); }
    void SetState(StateE state)                     { state_ = state; }
    void SetContext(void *arg)                      { context = arg; }
    void*GetContext()                               { return context; }
        
    void HandleRead();          ///<可读回调，Channel可读时调用它
    void HandleError();         ///错误回调
    void HandleClose();         ///<连接关闭函数，当handleRead中的::read返回0时调用它

    const socket::InetAddr& GetPeerAddr() const {  return peerAddr_; }
    const socket::InetAddr& GetLocalAddr() const { return localAddr_; }

    void ConnectEstablished();      ///<连接建立后，由TCPServer中的NewConnection调用此函数(记得转移到IO)
    void ConnectEstablishedInLoop();      
    //思考：为什么ConnectionDestory要传入一个ptr？这个ptr是拿来干嘛用？（延长TCPConnection生命周期）
    void ConnectDestroy(const TcpConnectionPtr &ptr);          ///<连接结束时最后调用的一个函数，only for IO thread

    void StopTimer(TimerID *TimerID);    

private:
    void HandleWrite();             ///<当一次写不完时，就会调用这个


private:
    EventLoop *loop_;
    
    std::unique_ptr<socket::Socket> socket_;
    std::unique_ptr<Channel> connectionChannel_;
    void* context;      ///<存放泛型数据(用于时间轮,存储weak_ptr<Entry>)    不安全！最好用boost::any

    const socket::InetAddr peerAddr_;
    const socket::InetAddr localAddr_;

    //由TCPServer或者TCPCLient传来的用户回调
    OnConnectionCallback           onConnectionCallback_;
    TcpConnectionReadCallBack      readCallback_;
    TcpConnectionWriteCallBack     writeCallback_;
    TcpConnectionWriteCompeleteCallBack writeCompCallback_;

    OnCloseCallback     onCloseCallback_;  ///<用于连接关闭时调用 

    Buffer inputBuffer_;    ///<输入缓冲区，输入指去读套接字，读的数据放在这
    Buffer outputBuffer_;   ///<输出缓冲区

    base::TimeStamp connectTime_;   ///<连接建立的时间

    StateE state_;
    bool isWriting;         ///<指出现在是否在写状态（用于数据没写完时）
};

}
