#pragma once
#include <bits/stdint-intn.h>
#include <bits/types/struct_itimerspec.h>
#include <bits/types/struct_timespec.h>
#include <bits/types/struct_timeval.h>
#include<sys/time.h>
#include <cstdio>
#include<time.h>
#include<string>
#include<string.h>

/*存放着以微秒计（1s = 1000,000微秒）的绝对时间！
 * 微秒的时间结构
 * struct timeval
 * {
 *      time_t tv_sec;
 *      long   tv_usec;
 * }
 * 相关函数：gettimeofday
 */
namespace base
{

class TimeStamp
{
public:
    TimeStamp()
        :timeval_(0)
    {

    }
    explicit TimeStamp(int64_t timeval)
        :timeval_(timeval)
    {

    }
    ~TimeStamp() = default;

    bool operator<(const TimeStamp &that) const 
    {
        return timeval_ < that.timeval_;
    }
    bool operator>(const TimeStamp &that) const 
    {
        return timeval_ > that.timeval_;
    }
    bool operator>=(const TimeStamp &that) const 
    {
        return timeval_ >= that.timeval_;
    }
    bool operator<=(const TimeStamp &that) const 
    {
        return timeval_ <= that.timeval_;
    }

    TimeStamp operator-(TimeStamp &that)
    {
        return TimeStamp(this->timeval_ - that.timeval_);
    }

    int64_t& timeGet()
    {
        return timeval_;
    }

    std::string GetFormatTime()
    {
        char buf [64] = { 0 };
        std::string formatTime = "";
        formatTime.reserve(64);

        struct tm t;
        ::bzero(&t, sizeof(tm));
        time_t timevalSecond = MicroSecondsToSeconds(this);
        localtime_r(&timevalSecond, &t);

        strftime(buf, sizeof buf, "%F.%T", &t);
        formatTime += buf;  ///<将精确到秒的时间传入formatTime
        formatTime += ".";
        formatTime += std::to_string(timeval_ % kMicroSecondsPerSecond);
        return formatTime;
    }
    static TimeStamp now()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return TimeStamp(TimeValToMicroSeconds(&tv));
    }

    static TimeStamp InValid()
    {
        return TimeStamp();
    }

    static bool IsValid(TimeStamp timeStamp)    ///<判断时间戳是否合法
    {
        return timeStamp.timeval_ > 0;
    }

    static void AddTime(TimeStamp& timeStamp, double seconds)
    {
        timeStamp.timeval_ += static_cast<int64_t>(seconds * kMicroSecondsPerSecond);
    }
    static struct timespec TimeStampToTimespec(TimeStamp* TimeStamp)
    {
        struct timespec tp;
        tp.tv_sec  = MicroSecondsToSeconds(TimeStamp);
        tp.tv_nsec = TimeStamp->timeval_ % kMicroSecondsPerSecond * kNanoSecondsPerMicroSecond;
        return tp;
    }
    
    static struct timeval TimeStampToTimeval(TimeStamp* TimeStamp)
    {
        struct timeval tv;
        tv.tv_sec  = MicroSecondsToSeconds(TimeStamp);
        tv.tv_usec = TimeStamp->timeval_ % kMicroSecondsPerSecond;
        return tv;
    }
    static TimeStamp DoubleSecondsToTimeStamp(double seconds)
    {
        return TimeStamp(static_cast<int64_t>(seconds * kMicroSecondsPerSecond));
    }
    static double   TimeStampToDoubleSeconds(TimeStamp timeStamp)
    {
        return static_cast<double>((double)timeStamp.timeval_ / (double)kMicroSecondsPerSecond);
    }

private:
    static time_t MicroSecondsToSeconds(TimeStamp* timeStamp)  ///<注意微秒转为秒会有精度丢失！！！！
    {
        return timeStamp->timeval_ / kMicroSecondsPerSecond;
    }
    static int64_t TimeValToMicroSeconds(struct timeval *val)
    {
        return static_cast<int64_t>(val->tv_usec) + static_cast<int64_t>(val->tv_sec * kMicroSecondsPerSecond);
    }
private:
    int64_t timeval_;
    const static int kMicroSecondsPerSecond = 1000 *1000;   ///<1秒等于1000*1000微妙
    const static int kNanoSecondsPerMicroSecond = 1000;     ///<1微秒等于1000纳秒

};

}
