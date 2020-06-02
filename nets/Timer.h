#pragma once
#include <bits/stdint-intn.h>
#include"Callbacks.h"
#include"../bases/TimeStamp.h"
namespace net
{

class Timer
{
public:
    Timer(TimerFunc timerFunc, base::TimeStamp nextExpiredTime, double interVal = 0.0)
        :expiredTime_(nextExpiredTime),
         timerFunc_(timerFunc),
         repeat_(interVal > 0.0 ? true: false),
         interVal_(interVal),
         sequence_(NumCreated_++)
     {
     }
    ~Timer() {}

    base::TimeStamp GetExpiredTime()
    {
        return expiredTime_;
    }
    
    int64_t GetTimerSequence()
    {
        return sequence_;
    }
    
    static void RunTimerCallBack(Timer *timer)
    {
        timer->timerFunc_();
    }
    void Stop()         {   repeat_ = false;   expiredTime_ = base::TimeStamp::InValid();}

    base::TimeStamp ReStart();

private:
    base::TimeStamp expiredTime_; ///<超时时刻（绝对时间）
    bool repeat_;           ///<是否为重复定时器
    double interVal_;       ///<超时时间间隔
    TimerFunc timerFunc_;
    int64_t sequence_;
    
    static int64_t NumCreated_ ;
};

}
