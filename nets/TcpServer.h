#pragma once
#include <array>
#include <memory>
#include<map>
#include <utility>
#include <vector>
#include"Acceptor.h"
#include"../bases/NonCopyable.h"
#include"TcpConnection.h"
#include"Callbacks.h"
#include"../bases/ThreadPool.h"

namespace net
{
class EventLoop;

class TcpServer : base::NonCopyable
{
public:
     TcpServer(EventLoop *loop, const char *ip = "127.0.0.1", int port = 6667);
     ~TcpServer();

     void SetOnConnectCallback(OnConnectionCallback cb)                 { onConnectCb_ = std::move(cb); }
     void SetReadCallback(TcpServerReadCallBack cb)                     { readCallback_ = std::move(cb);}
     void SetWriteCallback(TcpServerWriteCallBack cb)                   { writeCallback_ = std::move(cb);}
     void SetWriteCompleteCallback(TcpServerWriteCompeleteCallBack cb)  { writeCompeleteCallbck_ = std::move(cb);}
     void SetThreadNum(int threadNum)                                   { threadNum_ = threadNum; InitThreadPool(); }
     void NewConnection(int newConnFd);       ///<当新连接到来时，acceptor调用此函数
private:
     //思考：为什么RunInLoop可以调用TCPServer中的私有成员函数呢？
     //重要：请记住所有的连接建立的初始化和连接移除都要在自己的IO LOOP中完成！(为什么？)
     void RemoveConnection(const TcpConnectionPtr &ptr);        ///<当要关闭连接时，TcpConnection调用这个函数
     void IngoreSIGPIPE();          ///<忽视掉SIGPIPE

     void RemoveConnectionInLoop(const TcpConnectionPtr &ptr);

     EventLoop* SelectConnectEventLoop();    
     void InitThreadPool();
     void ThreadCallBack();         ///<单个线程的回调函数

private:
     EventLoop *loop_;
     Acceptor acceptor_;    ///<里面存放服务器的addr和最近一次连接的客户addr
     std::map<int, std::shared_ptr<TcpConnection>> ConnetionMap_;

     //用户回调
     OnConnectionCallback    onConnectCb_;
     TcpServerReadCallBack   readCallback_;
     TcpServerWriteCallBack  writeCallback_;
     TcpServerWriteCompeleteCallBack writeCompeleteCallbck_;

     base::MutexLock mutex_;        ///<保护以下三个pool变量
     bool isBeginPool_;
     int  threadNum_;
     std::unique_ptr<base::ThreadPool> pool_;
     std::vector<EventLoop*>  loopArray_;

};

}
