
#include"Timer.h"
#include <bits/stdint-intn.h>

using namespace base;
namespace net
{



base::TimeStamp Timer::ReStart()
{
    if(repeat_)
    {
        TimeStamp::AddTime(expiredTime_, interVal_);
        return expiredTime_;
    }
    else
       return TimeStamp::InValid();
}

int64_t Timer::NumCreated_ = 0;

}
