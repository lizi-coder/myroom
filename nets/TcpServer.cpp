#include "Channel.h"
#include "EventLoop.h"
#include "Callbacks.h"
#include "Socket.h"
#include "Acceptor.h"
#include"TcpConnection.h"
#include"TcpServer.h"
#include"../bases/Logging.h"
#include <functional>
#include <memory>
#include<sys/signal.h>
using namespace base;
namespace net
{
TcpServer::TcpServer(EventLoop *loop, const char *ip, int port)
            :loop_(loop),
             acceptor_(loop_, ip, port),
             mutex_(),
             isBeginPool_(false),
             threadNum_(0)
{
    acceptor_.SetReadCallBack(std::bind(&TcpServer::NewConnection, this ,std::placeholders::_1));
    IngoreSIGPIPE();
}
TcpServer::~TcpServer()
{

}



void TcpServer::NewConnection(int newConnFd)       ///<当新连接到来时，acceptor调用此函数
{
    LOG_DEBUG<<"TcpServer receive new Connetion! FD = "<<newConnFd;
    assert(ConnetionMap_.find(newConnFd) == ConnetionMap_.end());
    std::pair<int, std::shared_ptr<TcpConnection>> newConn =
        std::make_pair(newConnFd, std::shared_ptr<TcpConnection>(new TcpConnection(SelectConnectEventLoop(),newConnFd , acceptor_.GetClientAddr(),acceptor_.GetServerAddr())));

    newConn.second->SetReadCallback(readCallback_);
    newConn.second->SetWriteCallback(writeCallback_);
    newConn.second->SetWriteCompleteCallback(writeCompeleteCallbck_);
    newConn.second->SetOnConnectionCallback(onConnectCb_);

    newConn.second->SetCloseCallback(std::bind(&TcpServer::RemoveConnection, this, std::placeholders::_1));

    ConnetionMap_.insert(newConn);

    newConn.second->ConnectEstablished();
}


void TcpServer::RemoveConnection(const TcpConnectionPtr &ptr)
{
    LOG_TRACE<<"In Function:" <<__func__<<" shared_ptr<TcpConnection>.use_count is "<<static_cast<int>(ptr.use_count());
    LOG_DEBUG<<"Now Run RemoveConnection, closecb...";
    loop_->RunInLoop(std::bind(&TcpServer::RemoveConnectionInLoop, this, ptr));
}

void TcpServer::IngoreSIGPIPE()
{
    ::signal(SIGPIPE, SIG_IGN);
}

void TcpServer::RemoveConnectionInLoop(const TcpConnectionPtr &ptr)
{
    
     LOG_TRACE<<"In Function:" <<__func__<<"begin shared_ptr<TcpConnection>.use_count is "<<static_cast<int>(ptr.use_count());
     int removeFd = ptr->GetSocketFd();
     {
     auto it = ConnetionMap_.find(removeFd);
     //assert(it != ConnetionMap_.end());
     if(it != ConnetionMap_.end())
         ConnetionMap_.erase(it);           ///<思考此处TcpConneciton的引用计数是多少？
     }
     LOG_TRACE<<"In Function:" <<__func__<<" shared_ptr<TcpConnection>.use_count is "<<static_cast<int>(ptr.use_count());

     ptr->GetLoop()->RunInLoop(std::bind(&TcpConnection::ConnectDestroy, ptr.get(), ptr));     ///<将销毁任务转移到TcpConnection的IO thread中执行
}

EventLoop* TcpServer::SelectConnectEventLoop()
{
    if(isBeginPool_)
        assert(loopArray_.size() == threadNum_);
    static int round_robin = 0;
    if(loopArray_.size() == 0)
        return loop_;
    else if(loopArray_.size() == 1)
        return loopArray_[0];
    else
    {
        round_robin = (round_robin + 1) % loopArray_.size();
        //LOG_TRACE<<"Now  this TcpConnection RoundRobin Select "<<round_robin;
        return loopArray_[round_robin];
    }

}

void TcpServer::InitThreadPool()    ///<
{
    if(isBeginPool_ == false)
    {
        isBeginPool_ = true;
        pool_.reset(new ThreadPool(std::bind(&TcpServer::ThreadCallBack, this), threadNum_));
        pool_->start();
    }
}


void TcpServer::ThreadCallBack()
{
    LOG_INFO<<"ThreadPool::::Thread "<<CurrentThread::Tid()<<" ,Name:"<<CurrentThread::GetThreadName()<<" Create Ok!";
    EventLoop loop;
    {
    MutexLockGuard lock(mutex_);
    loopArray_.push_back(&loop);
    }

    loop.Loop();
}


}
