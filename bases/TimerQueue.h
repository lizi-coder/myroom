#pragma once


#include <memory>
#include <utility>
#include<set>
#include<vector>

#include"../bases/TimeStamp.h"
#include "Channel.h"
#include "Timer.h"
namespace net
{

class TimerQueue
{
public:
    TimerQueue() {}
    ~TimerQueue() {}



    void AddTimer(Timer *timer);
    void TimerFdInit();
    void RunTimerCallBacks();
    inline void AssertTheSameLoop();
    bool Insert(Timer *timer);      ///<插入一个定时器
    void Reset(std::vector<Timer*> &ExpiredTimer);               ///<用于delete timer以及间隔定时器的重新插入
    void ChangeSettingTime(base::TimeStamp when);       ///<改变定时器时间
    void GetExpired(base::TimeStamp ExpiredTime);             ///<获取所有已到定时时间的定时器

private:



private:
    EventLoop *loop_;
    std::unique_ptr<Channel> timerChannel_;
    int timerfd_;




    typedef std::pair<base::TimeStamp, Timer*> TimePair;
    typedef std::set<TimePair> TimePairSet;
    typedef std::pair<Timer*, int64_t> TimeSeqPair;
    typedef std::set<TimeSeqPair> ActiveTimerSet;

    TimePairSet timerSet_;
    ActiveTimerSet activeTimerSet_;

    std::vector<Timer*> expiredTimer_;
    

};

}
