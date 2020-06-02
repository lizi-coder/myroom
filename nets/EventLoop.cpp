#include<vector>
#include<sys/eventfd.h>
#include "Callbacks.h"
#include"EventLoop.h"
#include"Channel.h"
#include"../bases/Logging.h"
#include"../bases/MutexLockGuard.h"
#include "Timer.h"
#include "TimerID.h"
#include "TimerQueue.h"
using namespace base;
namespace net
{

void EventLoop::Loop()
{
    assert(tid_ == CurrentThread::Tid());
    while(!quit_)
    {
        activeChannels.clear();
        poll->Poll(activeChannels);
        int nready = activeChannels.size();
        for(auto c : activeChannels)
        {
            c->HandleEvent();
            LOG_TRACE<<"now HandleEvent run!";
        }
        RunningIOcb();
    }

}

void EventLoop::UpdateChannel(Channel* target, int events )
{
    assert(target != NULL);
    poll->UpdateChannel(target, events);
}

void EventLoop::RemoveChannel(Channel* target)
{
    assert(target != NULL);
    assert(target->GetEvents() == 0 && target->GetRevents() == 0);
    poll->RemoveChannel(target);
}


void EventLoop::RunInLoop(IOThreadCallBack IOcb)
{
    if(tid_ == CurrentThread::Tid())
        IOcb();
    else
        QueueInLoop(IOcb);
}


void EventLoop::QueueInLoop(IOThreadCallBack IOcb)
{
    {
    MutexLockGuard lock(mutex_);
    callBackQueue_.push_back(IOcb);
    }
    if(tid_ != CurrentThread::Tid() || isRunningIOCallBack_)
        WakeupFd();
}

void EventLoop::RunningIOcb()   ///<巧妙之处：栈上定义一临时容器和loop中的容器进行swap以减小临界区长度
{
    assert(CurrentThread::Tid() == tid_);       //
    std::vector<IOThreadCallBack> tempCallBackQueue_;
    {
    MutexLockGuard lock(mutex_);
    tempCallBackQueue_.swap(callBackQueue_);
    }
    isRunningIOCallBack_ = true;
    for(auto IOcb : tempCallBackQueue_)
        IOcb();
    isRunningIOCallBack_ = false;
}
void EventLoop::WakeupInit()
{
    wakeupFd_ = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    assert(wakeupFd_ >= 0);

    wakeupChannel_.reset(new Channel(this, wakeupFd_));
    wakeupChannel_->SetReadCallBack(std::bind(&EventLoop::ReadWakeupFd, wakeupFd_));
    wakeupChannel_->EnableReading();
}
void EventLoop::WakeupFd()
{
    char buf[8] = {'n','o','t','i','f','y','s','!'};
    ::write(wakeupFd_, buf, 8);
}

void EventLoop::ReadWakeupFd(int WakeupFd)
{
    char buf[8];
    ::read(WakeupFd, buf, 8);
}

//过aferHowMuchSeconds秒后执行一次cb
TimerID EventLoop::RunAfter(EventLoop *loop, TimerFunc cb, double afterHowMuchSeconds)
{
    TimeStamp expiredTime = TimeStamp::now();
    TimeStamp::AddTime(expiredTime, afterHowMuchSeconds);
    Timer* timer = new Timer(std::move(cb), expiredTime, 0.0);
    loop->RunInLoop(std::bind(&EventLoop::AddTimer,this, timer));

    return TimerID(timer, timer->GetTimerSequence());
}

//在when的时候执行一次cb
TimerID EventLoop::RunAt(EventLoop *loop, TimerFunc cb, TimeStamp when)
{
    Timer* timer = new Timer(std::move(cb),when);
    //double diff = TimeStamp::TimeStampToDoubleSeconds((TimeStamp::now() - when));
    loop->RunInLoop(std::bind(&EventLoop::AddTimer, this, timer));

    return TimerID(timer, timer->GetTimerSequence());
}
//从过了firstSeconds秒后，每隔interval秒执行一次cb
TimerID EventLoop::RunEvery(EventLoop *loop, TimerFunc cb, double firstSeconds, double interval)
{
    TimeStamp expiredTime = TimeStamp::now();
    TimeStamp::AddTime(expiredTime, firstSeconds);
    Timer* timer = new Timer(std::move(cb), expiredTime, interval);
    loop->RunInLoop(std::bind(&EventLoop::AddTimer,this, timer));

    return TimerID(timer, timer->GetTimerSequence());
}

//only for IO thread:通知定时队列产生定时器并添加
void EventLoop::AddTimer(Timer *timer)
{
    LOG_TRACE<<"now EventLoop begin addtimer...";
    timerQueue_->AddTimer(timer);
}
void EventLoop::CancelTimer(TimerID *id)
{
    RunInLoop(std::bind(&EventLoop::CancelTimerInLoop, this, id));
}
void EventLoop::CancelTimerInLoop(TimerID *id)
{
    timerQueue_->CancelTimer(id);
}

}   //END OF namespace net
