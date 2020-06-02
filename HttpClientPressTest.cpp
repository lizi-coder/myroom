#include <cstdio>
#include <iostream>
#include"./bases/ThreadPool.h"
#include"./nets/TcpClient.h"
#include "bases/Logging.h"
#include "bases/TimeStamp.h"
#include "nets/Buffer.h"
#include "nets/Callbacks.h"
#include "nets/EventLoop.h"


using namespace std;
using namespace base;
using namespace net;

char buf[1024] = { 0 };
FILE *file = fopen("./HTTPReq", "rw");
int n = fread(buf, 1, 1024, file);
string req(buf, buf + n);


void MyTimer(const TcpConnectionPtr &ptr)
{
    ptr->Send(req);
}


void OnConnection(const TcpConnectionPtr &ptr);
void reads(const TcpConnectionPtr &ptr, Buffer &buf, TimeStamp &recvtime);

void client(char **arg)
{
    EventLoop loop;
    TcpClient client(&loop, arg[1],atoi(arg[2]) );
    client.SetOnConnectCallback(OnConnection);
    client.SetReadCallback(reads);
    client.Start();
    loop.Loop();
}

void OnConnection(const TcpConnectionPtr &ptr)
{
    ptr->GetLoop()->RunEvery(ptr->GetLoop(), std::bind(MyTimer, ptr), 3 ,1.0);
}

void reads(const TcpConnectionPtr &ptr, Buffer &buf, TimeStamp &recvtime)
{
    LOG_INFO<<"RecvTime:"<<recvtime.GetFormatTime();
}




int main(int argc, char** argv)
{
    if(argc < 3)
    {
        printf("Usage: ./myclient ip port [ThreadNumber]");
        return -1;
    }

    int threadNum = 100;
    if(argc == 4)
        threadNum = atoi(argv[3]);

    EventLoop loop;
    ThreadPool pool(bind(client, argv), threadNum);
    pool.start();

    loop.Loop();
    




    std::cout << "Hello world" << std::endl;
    return 0;
}

