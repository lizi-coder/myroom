#include <iostream>
#include <sys/poll.h>
#include <unistd.h>
#include "bases/CurrentThread.h"
#include"nets/EventLoop.h"
#include"bases/Logging.h"

/**
 * 测试目标：测试eventloop中的IO任务队列工作情况
 */


using namespace base;

void task1()
{
    LOG_INFO<<"task1 occur"<<"CurrentTid is"<<CurrentThread::Tid();
}
void task2()
{
    LOG_INFO<<"task2 occur"<<"CurrentTid is"<<CurrentThread::Tid();
}
void task3()
{
    LOG_INFO<<"task3 occur"<<"CurrentTid is"<<CurrentThread::Tid();
}

void taskMain()
{
    LOG_INFO<<"taskmain occur"<<"CurrentTid is"<<CurrentThread::Tid();
}


void* thread1(void*arg)
{
    net::EventLoop *loop = (net::EventLoop*)arg;
    
    while(1)
    {
        ::poll(NULL,0,5000);
        loop->RunInLoop(task1);
    }

    return NULL;
}

void* thread2(void*arg)
{
    net::EventLoop *loop = (net::EventLoop*)arg;
    while(1)
    {
        ::poll(NULL,0,2000);
        loop->RunInLoop(task2);
    }

    return NULL;
}

void* thread3(void*arg)
{
    net::EventLoop *loop = (net::EventLoop*)arg;
    while(1)
    {
        ::poll(NULL,0,8000);
        loop->RunInLoop(task3);
    }

    return NULL;
}

int main()
{
    Logging::SetLogRank("INFO");

    net::EventLoop loop;

    loop.RunInLoop(taskMain);

    std::cout << "now begin create task" << std::endl;
    
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, thread1, (void*)&loop);
    pthread_create(&tid2, NULL, thread2, (void*)&loop);
    pthread_create(&tid3, NULL, thread3, (void*)&loop);
    
    loop.QueueInLoop(taskMain);
    loop.QueueInLoop(taskMain);
    loop.QueueInLoop(taskMain);
    loop.QueueInLoop(taskMain);
    loop.QueueInLoop(taskMain);
    loop.QueueInLoop(taskMain);

    loop.Loop();

    std::cout << "Hello world" << std::endl;
    return 0;
}

