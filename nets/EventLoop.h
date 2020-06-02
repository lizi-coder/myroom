#pragma once
#include <memory>
#include<unistd.h>
#include <vector>
#include"Callbacks.h"
#include "Poller.h"
#include"EpollPoller.h"
#include"PollPoller.h"
#include"../bases/CurrentThread.h"
#include"../bases/MutexLock.h"
#include"TimerQueue.h"
#include"TimerID.h"


///<EPOLL和POLL的选择

namespace net
{
//const bool useEpoll = false; ///<原作是通过环境变量控制是选择epoll还是poll,这里为了测试方便默认用poll
class EventLoop
{
public:
    EventLoop()         ///<这边如何实现poll和epoll的多态？
        :tid_(base::CurrentThread::Tid()),
         poll(0 ? static_cast<Poller*>(new EpollPoller()) : static_cast<Poller*>(new PollPoller())),
         wakeupFd_(0),
         wakeupChannel_(),
         mutex_(),
         isRunningIOCallBack_(false),
         timerQueue_(new TimerQueue(this)),
         quit_(false)
    {
        WakeupInit();
    }
    ~EventLoop() {}
    
    void AssertThreadInLoop() const
    {
        assert(tid_ == base::CurrentThread::Tid());
    }

    void Loop();
    void Quit()
    {
        AssertThreadInLoop();
        quit_ = true;
    }
    pid_t GetLoopTid() const
    {
        return tid_;
    }


    void UpdateChannel(Channel* target, int events = 0);
    void RemoveChannel(Channel* target);

    /**
     * 异步IO任务队列函数群
     */
    void RunInLoop(IOThreadCallBack);       ///<作用：在IO线程中执行某一任务，是线程安全的
    void QueueInLoop(IOThreadCallBack);     ///<作用：将任务排入队伍
private:
    void RunningIOcb();                     ///<执行队伍中的cb
    void WakeupInit();                      ///<初始化，包括创建eventfd并设置监听、设置其回调读函数
    void WakeupFd();                        ///<唤醒，相当于当时用管道时在写端写一个字节的作用
    static void ReadWakeupFd(int WakeupFd);

    /**
     * 定时器函数群
     */
public:
    TimerID RunAfter(EventLoop *loop, TimerFunc cb, double afterHowMuchSeconds);
    TimerID RunAt(EventLoop *loop, TimerFunc cb, base::TimeStamp when);
    TimerID RunEvery(EventLoop *loop, TimerFunc cb, double firstSeconds, double interval);
    void CancelTimer(TimerID *id);
private:
    void AddTimer(Timer *timer);
    void AddTimerInLoop();
    void CancelTimerInLoop(TimerID *id);


private:
    std::unique_ptr<Poller> poll;
    std::vector<Channel*> activeChannels;

    std::vector<IOThreadCallBack> callBackQueue_;
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;    ///<替代了原先管道通信，其回调函数不负责调用IOcb！
    base::MutexLock mutex_;
    bool isRunningIOCallBack_; 

    //定时器相关
    std::unique_ptr<TimerQueue> timerQueue_;
    
    bool quit_;

    pid_t tid_;
};

}
