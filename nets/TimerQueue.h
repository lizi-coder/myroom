#pragma once


#include <memory>
#include <utility>
#include<set>
#include<vector>

#include"../bases/TimeStamp.h"
#include "Channel.h"
#include "Timer.h"
#include "TimerID.h"
#include"../bases/MutexLock.h"
namespace net
{

class TimerQueue
{
public:
    typedef std::pair<base::TimeStamp, Timer*> TimePair;
    typedef std::set<TimePair> TimePairSet;
    typedef std::pair<Timer*, int64_t> TimeSeqPair;
    typedef std::set<TimeSeqPair> ActiveTimerSet;
    TimerQueue(EventLoop *loop)
        :loop_(loop),
	 timerChannel_(),
         timerfd_(0)
    {
        TimerFdInit();
    }

    ~TimerQueue() {}



    void AddTimer(Timer *timer);
    void TimerFdInit();
    void RunTimerCallBacks();
    inline void AssertTheSameLoop();
    bool Insert(Timer *timer);      ///<插入一个定时器
    void Reset(std::vector<TimePair> &);               ///<用于delete timer以及间隔定时器的重新插入
    void ChangeSettingTime(base::TimeStamp when);       ///<改变定时器时间
    void GetExpired(base::TimeStamp ExpiredTime);             ///<获取所有已到定时时间的定时器


    void CancelTimer(TimerID *id);

private:

private:
    EventLoop *loop_;
    std::unique_ptr<Channel> timerChannel_;
    int timerfd_;

    TimePairSet timerSet_;
    ActiveTimerSet activeTimerSet_;

    std::vector<TimePair> expiredTimer_;  ///<GetExpired取出的元素是pair类型！

    base::MutexLock mutex_;
};

}
