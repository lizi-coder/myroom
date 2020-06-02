#pragma once

#include <bits/stdint-intn.h>
#include"Timer.h"

namespace net
{

class TimerID
{
public:
    TimerID(Timer *timer, int64_t sequence)
        :sequence_(sequence),
         timer_(timer)
    {}
    TimerID()
        :sequence_(-1),
         timer_(NULL)
    {}

    TimerID(const TimerID &that)
    {
        sequence_ = that.sequence_;
        timer_    = that.timer_;
    }
    TimerID& operator=(const TimerID &that)
    {
        if(this == &that)
            return *this;
        sequence_ = that.sequence_;
        timer_    = that.timer_;
        return *this;
    }


    ~TimerID() {}

    int64_t GetTimerID()
    {
        return sequence_;
    }
    Timer* GetTimer()
    {
        return timer_;
    }

private:
    int64_t sequence_;      ///<定时器序列号
    Timer* timer_;

};

}
