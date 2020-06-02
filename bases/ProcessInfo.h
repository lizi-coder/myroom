#pragma once
#include<unistd.h>
#include<string>
#include<vector>
#include"TimeStamp.h"

namespace base
{
namespace ProcessInfo
{

struct CpuTime;
//获取进程相关信息
//获取pid

pid_t Getpid();
std::string GetpidAsString();
uid_t Getuid();
//获取主机名字
std::string GetHostName();

//获取进程相关状态
int GetThreadTotalNum();
std::vector<pid_t> GetAllThreadPid();
int GetOpendFileNum();
//获取可执行文件的路径在哪
std::string GetExecPath();



//获取进程运行频率、cpu时间以及进程启动时间

const double GetTicksPerSecond();
CpuTime GetCpuTime();
const TimeStamp GetStartTime();






//const int TicksPerSecond = ::con




struct CpuTime
{
    CpuTime()
        :userTime_(0.0),
         sysTime_(0.0)
    {}
    CpuTime(double usertime, double systime)
        :userTime_(usertime),
         sysTime_(systime)
    {}

    double GetUserTime() { return userTime_; }        
    double GetSysTime()  { return sysTime_;  }        

    double TotalTime()   { return userTime_ + sysTime_; }

private:
    double userTime_;
    double sysTime_;
};









}
}
