#include <functional>
#include <iostream>
#include "bases/TimeStamp.h"
#include "nets/Buffer.h"
#include "nets/Callbacks.h"
#include"nets/EventLoop.h"
#include"nets/TcpServer.h"
#include"bases/Logging.h"
#include "nets/TimerID.h"
#include"ulimit.h"
using namespace net;
using namespace base;

static TimerID id;
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
       // id = loop->RunEvery(loop, std::bind(&::Timer, conn), 3.2, 1.0);
    std::string s = "hello now is";
    TimeStamp now = TimeStamp::now();
    s += now.GetFormatTime();
    conn->Send(s);
    }
    else if(conn->Disconnected())
    {
        LOG_INFO<<"Connection:"<<conn->GetSocketFd()<<" shutdown!";
//        conn->StopTimer(&id);
    }
    else
    {
        LOG_WARN<<"other Connetion state occur!";
    }
}

void OnRead(const TcpConnectionPtr &conn, Buffer &buffer, TimeStamp recvTime)
{
    LOG_INFO<<"Server Recv "<<buffer.DataNumber()<<" bytes,context is "<<buffer.RetrieveAllAsString()<<" SOCKET:"<<conn->GetSocketFd();
    LOG_INFO<<"Buffer Capacity is "<<buffer.BackEndCapacity();
}

//测试一个类的private成员函数是否能在loop任务队列里做？
class Test_Internal
{
public:
    Test_Internal(EventLoop *loop) { loop->RunInLoop(std::bind(&Test_Internal::TestPrivate, this)); }

    void test(std::function<void(Test_Internal*)> *cb)
    {
        *cb = std::bind(&Test_Internal::TestPrivate, this);
    }
private:
    void TestPrivate()
    {
        LOG_INFO<<"INTO TestPrivate ok!";
    }
};


int main()
{
    Logging::SetLogRank("OFF");
    ulimit(UL_SETFSIZE, 999999);
    EventLoop loop;

    TcpServer server(&loop);
    server.SetReadCallback(OnRead);
    server.SetOnConnectCallback(OnConnetion);
    server.SetThreadNum(10);

    loop.Loop();

    std::cout << "Hello world" << std::endl;
    return 0;
}

