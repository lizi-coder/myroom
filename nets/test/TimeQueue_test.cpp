#include<iostream>
#include <pthread.h>
#include"./bases/Logging.h"
#include"nets/EventLoop.h"

using namespace std;
using namespace base;
using namespace net;

/*定时器测试
 *
 * BUG1:两线程或多线程同时设定一样的RunEvery会断言不过
 */

void timer1()
{
    LOG_INFO<<"timer1 runs!";
}
void timer2()
{
    LOG_INFO<<"timer2 every runs!";
}

void timer3()
{
    LOG_INFO<<"timer3 runs!";
}

void* thread1(void *arg)
{
    EventLoop *loop = (EventLoop*)arg;
    loop->RunEvery(loop, timer2, 5.7, 2.2);
    return NULL;
}

void* thread2(void *arg)
{
    EventLoop *loop = (EventLoop*)arg;
    loop->RunEvery(loop, timer2, 3.5, 2.2);
    return NULL;
}

void* thread3(void *arg)
{
    EventLoop *loop = (EventLoop*)arg;
    loop->RunEvery(loop, timer2, 4.8, 2.2);
    return NULL;
}
int main()
{
    EventLoop loop;
    
    Logging::SetLogRank("DEBUG");

    LOG_INFO<<"now run after";

    loop.RunAfter(&loop, timer1, 4.3);
    loop.RunAfter(&loop, timer1, 5.3);
    //loop.RunEvery(&loop, timer2, 5.6, 2.2);

    loop.RunEvery(&loop, timer2, 5.6, 2.2);

    pthread_t tid;
    pthread_create(&tid, NULL, thread1, (void*)&loop);
    pthread_create(&tid, NULL, thread2, (void*)&loop);
    pthread_create(&tid, NULL, thread3, (void*)&loop);

    loop.RunInLoop(timer3);

    loop.Loop();

    return 0;

}

