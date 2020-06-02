#pragma once

/**
 * 对ListenFd的抽象：服务器连接类,一般来说普通的服务器就一个这个类
 */
#include "Channel.h"
#include "InetAddr.h"
#include"Socket.h"
#include"EventLoop.h"
#include"Callbacks.h"
#include"../bases/NonCopyable.h"
#include <memory>


namespace net
{
class Acceptor : base::NonCopyable
{
public:
    Acceptor(EventLoop *loop, const char *IPAddr, int port);
    ~Acceptor() = default;

    void SetReadCallBack(AcceptorCallBack cb);
    void AcceptAndCallback();      ///<连接并调用其上层的读回调

    socket::InetAddr GetClientAddr()
    {
        return clientAddr_;
    }

    socket::InetAddr GetServerAddr() const
    {
        return serverAddr_;
    }

private:
    void Listen();
    int Accept();

    EventLoop *loop_;
    socket::Socket listenFd_;
    socket::InetAddr serverAddr_;
    socket::InetAddr clientAddr_;           ///<存储最近一次连接的peerAddr
    std::unique_ptr<Channel> listenChannel_;
    AcceptorCallBack readCallBack_;

    int foolishFd;      ///<闲余fd，用于处理连接过多的情况

    static int kDefaultListenValue;
};

}
