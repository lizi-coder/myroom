#include <algorithm>
#include<assert.h>
#include <bits/stdint-intn.h>
#include <bits/types/struct_itimerspec.h>
#include <cstdint>
#include <ctime>
#include <functional>
#include <iterator>
#include<sys/timerfd.h>
#include <utility>
#include <vector>
#include "Channel.h"
#include"EventLoop.h"
#include "Timer.h"
#include"TimerID.h"
#include"TimerQueue.h"
#include"../bases/Logging.h"
#include"../bases/MutexLockGuard.h"
using namespace base;
namespace net
{

bool operator<(TimerQueue::TimePair& left, TimerQueue::TimePair& right)
{
    return left.first < right.first && left.second < right.second; 
}

bool operator<(TimerQueue::TimeSeqPair& left, TimerQueue::TimeSeqPair& right)
{
    return left.first < right.first && left.second < right.second; 
}

//定时器添加函数
/*
void TimerQueue::AddTimer(EventLoop *loop,TimerFunc TimerFuncCB, base::TimeStamp when)
{
    assert(loop == loop_);
    Timer*  timer = new Timer(std::move(TimerFuncCB),when);
    bool lastestTimerTimeChange =  Insert(timer);
    if(lastestTimerTimeChange)
        ChangeSettingTime(when);

    return TimerID(timer, timer->GetTimerSequence());
}


void TimerQueue::AddTimer(EventLoop *loop,TimerFunc TimerFuncCB , double howMuchTimeFromNow, double interval_)
{
    assert(loop == loop_);
    TimeStamp expiredTime = TimeStamp::now();
    TimeStamp::AddTime(expiredTime, howMuchTimeFromNow);
    Timer*  timer = new Timer(std::move(TimerFuncCB), expiredTime, interval_);

    bool lastestTimerTimeChange =  Insert(timer);
    if(lastestTimerTimeChange)
        ChangeSettingTime(expiredTime);
    return TimerID(timer, timer->GetTimerSequence());
}
*/
void TimerQueue::AddTimer(Timer *timer)
{
    bool lastestTimerTimeChange =  Insert(timer);
    if(lastestTimerTimeChange)
        ChangeSettingTime(timer->GetExpiredTime());
}
void TimerQueue::TimerFdInit()
{
    timerfd_ = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
    assert(timerfd_ >= 0);
    LOG_TRACE<<"TimerFd Create";
    timerChannel_.reset(new Channel(loop_, timerfd_));
    assert(timerChannel_.get() != NULL);
    timerChannel_->SetReadCallBack(std::bind(&TimerQueue::RunTimerCallBacks, this));
    timerChannel_->EnableReading();
}

void TimerQueue::RunTimerCallBacks()
{
    assert(loop_->GetLoopTid() == CurrentThread::Tid());
    char buf[8];
    int n = ::read(timerfd_, buf, 8);
    assert(n == 8);

    expiredTimer_.clear();
    GetExpired(TimeStamp::now());
    assert(expiredTimer_.size() > 0);
    for(auto &timerSetPair : expiredTimer_)
    {
        Timer::RunTimerCallBack(timerSetPair.second);     ///<遍历活跃定时器并调用其cb
    }
    Reset(expiredTimer_);
}

inline void TimerQueue::AssertTheSameLoop()
{
    assert(loop_->GetLoopTid() == base::CurrentThread::Tid());
}

bool TimerQueue::Insert(Timer *timer)      ///<插入一个定时器
{
    bool lastestTimerTimeChange = false;    ///<最早的定时器时间是否发生改变的标志位
    if(timerSet_.empty() ||( timerSet_.begin()->first > timer->GetExpiredTime() ))
        lastestTimerTimeChange = true;

    assert(timerSet_.size() == activeTimerSet_.size());
    TimeStamp timestamp = timer->GetExpiredTime();
    TimePair timepair = { timestamp, timer };   ///<为待插入的定时器造pair
    TimeSeqPair TimeSeqPair = {timer , timer->GetTimerSequence()};

    timerSet_.insert(timepair);
    activeTimerSet_.insert(TimeSeqPair);
    
    assert(timerSet_.size() == activeTimerSet_.size());
    LOG_TRACE<<"Insert New Timer OK!";
    return lastestTimerTimeChange;
}

void TimerQueue::Reset(std::vector<TimePair> &ExpiredTimer)                   ///<用于delete timer以及间隔定时器的重新插入
{
    TimeStamp nextExpired;
    assert(ExpiredTimer.size() > 0);
    for(auto& timerSetPair : ExpiredTimer)
    {
        Timer *timer = timerSetPair.second;
        TimeStamp NextTimeStamp = timer->ReStart(); ///<该定时器的下一个定时时间（如果是一次性定时器，
        if(TimeStamp::IsValid(NextTimeStamp))   ///<重复运行的定时器
        {
            Insert(timer);
        }
        else
        {
            delete timer;
        }
    }
    assert(activeTimerSet_.size() == timerSet_.size());
    if(!activeTimerSet_.empty())    ///<只要还有定时器，就重新设置超时时刻
        nextExpired = timerSet_.begin()->first;

    if(TimeStamp::IsValid(nextExpired))
        ChangeSettingTime(nextExpired);
}

//当我们不想定时器触发时，可以传入一个非法的when值
void TimerQueue::ChangeSettingTime(TimeStamp when)       ///<改变定时器时间
{
    //设置时间为when，注意时间转化
    TimeStamp now = TimeStamp::now();
    TimeStamp diff(when - now);         ///<when指代下一次定时器发生的时间，肯定比now大

    struct itimerspec oldtime, newtime;
    bzero(&oldtime, sizeof(itimerspec));
    bzero(&newtime, sizeof(itimerspec));

    if(!TimeStamp::IsValid(when))           ///<如果when不合法，则不再设置定时时间，即fd不再触发
        newtime.it_value    = {0 ,0};
    else
        newtime.it_value    = TimeStamp::TimeStampToTimespec(&diff);
    newtime.it_interval = { 0,0 };

    int isOK = timerfd_settime(timerfd_, CLOCK_REALTIME, &newtime, &oldtime);
    if(isOK != 0)
    {
        LOG_ERROR<<"TimerFd SetTime Error!";
    }

    LOG_TRACE<<"now Setting new lastest time:"<<TimeStamp::TimeStampToDoubleSeconds(diff);
}

///<这部分对容器操作不熟练！！！！
void TimerQueue::GetExpired(TimeStamp ExpiredTime)              ///<获取所有已到定时时间的定时器
{
    assert(timerSet_.size() == activeTimerSet_.size());

    TimePair setpair = { ExpiredTime, reinterpret_cast<Timer*>(UINTPTR_MAX) };
    TimePairSet::iterator end =  timerSet_.lower_bound(setpair);

    std::copy(timerSet_.begin(), end , std::back_inserter(expiredTimer_));   ///<学习copy的用法
    timerSet_.erase(timerSet_.begin(), end);

    //有没有更好的办法删除activeTimerSet中的元素？
    
    for(auto &timerSetPair : expiredTimer_)
    {
        TimeSeqPair active = { timerSetPair.second, timerSetPair.second->GetTimerSequence() };
        int n = activeTimerSet_.erase(active);
        assert(n == 1);
    }
    
    assert(timerSet_.size() == activeTimerSet_.size());
}

void TimerQueue::CancelTimer(TimerID *id)
{
    MutexLockGuard lock(mutex_);
    if(id == NULL)
        LOG_FATAL<<"InValid Argument TimerID!";

    LOG_INFO<<"Now Remove Timer!";
    TimeStamp nextExpired = id->GetTimer()->GetExpiredTime();
    Timer *timer  = id->GetTimer();
    int64_t seq = id->GetTimer()->GetTimerSequence();

    TimeSeqPair p1{ timer, seq   };
    TimePair    p2{ nextExpired, timer };
    auto targetPlace = activeTimerSet_.find(p1);

    if(targetPlace != activeTimerSet_.end())
    {
        if(activeTimerSet_.size() == 1)         ///<如果只有一个定时器，删除完就只有0个了
            ChangeSettingTime(TimeStamp::InValid());
        else if(targetPlace == activeTimerSet_.begin())
            ChangeSettingTime((*timerSet_.begin()).first);

        activeTimerSet_.erase(p1);
        timerSet_.erase(p2);
    }
    if(timer)
        delete timer;
    

}

}

