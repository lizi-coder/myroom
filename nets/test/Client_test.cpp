#include <cstdlib>
#include <iostream>
#include <functional>
#include <sys/poll.h>
#include"nets/TcpClient.h"
#include"bases/TimeStamp.h"
#include"nets/EventLoop.h"
#include"bases/Logging.h"
#include"bases/ThreadPool.h"
#include"ulimit.h"
using namespace net;
using namespace base;
void Timer(const TcpConnectionPtr &conn)
{
    std::string s = "hello now is";
    TimeStamp now = TimeStamp::now();
    s += now.GetFormatTime();
    conn->Send(s);
}
void OnConnetion(const TcpConnectionPtr &conn)
{
    if(conn->Connected())
    {
        LOG_INFO<<"New Connection comes!"<<conn->GetPeerAddr().InetAddrToFormatString();
        EventLoop *loop = conn->GetLoop();
        loop->RunEvery(loop, std::bind(&::Timer, conn), 3.2, 1.0);
    }
    else if(conn->Disconnected())
    {
        LOG_INFO<<"Connection:"<<conn->GetSocketFd()<<" shutdown!";
    }
    else
    {
        LOG_WARN<<"other Connetion state occur!";
    }
}

void OnRead(const TcpConnectionPtr &conn, Buffer &buffer, TimeStamp recvTime)
{
    LOG_INFO<<"client recv "<<buffer.DataNumber()<<" bytes,context is "<<buffer.RetrieveAllAsString();
    LOG_INFO<<"Buffer Capacity is "<<buffer.BackEndCapacity();
}

void DoPressClient()        ///<创建N个线程干这事
{


    EventLoop loop;
    TcpClient client(&loop, "127.0.0.1", 6667);
    client.SetReadCallback(OnRead);
    client.SetOnConnectCallback(OnConnetion);
    client.Start();
    loop.Loop();
}


int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout<<"usage ./myclient ConnectedNum"<<std::endl;
        return -1;
    }
    EventLoop loop;
    ulimit(UL_SETFSIZE, 99999);
    ThreadPool pool(DoPressClient, atoi(argv[1]));
    pool.start();
    loop.Loop();

    std::cout << "Hello world" << std::endl;
    return 0;
}

