#include"ProcessInfo.h"
#include "File.h"
#include "TimeStamp.h"
#include<assert.h>
#include <cctype>
#include<dirent.h>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <vector>
#include<sys/times.h>

using namespace std;
namespace base
{
namespace ProcessInfo
{

const TimeStamp startTime = TimeStamp::now();
const pid_t ProcessPid    = ::getpid();
const double clickTicks   = ::sysconf(_SC_CLK_TCK);

vector<pid_t>* ThreadPidList = NULL;

int FdNumber = 0;

/**
 * 这部分是目录搜索函数，仅供内部使用
 */
//文件描述符过滤器，用于统计当前进程开的文件描述符个数
typedef int(*Filter)(const struct dirent *);

int FdNumberFilter(const struct dirent * dir)
{
    if(::isdigit( dir->d_name[0]))
        FdNumber++;
    return 0;
}

int ThreadPidFilter(const struct dirent *dir)
{
    if(ThreadPidList && ::isdigit(dir->d_name[0]))
        ThreadPidList->emplace_back(::atoi(dir->d_name));
    return 0;
}


int Scandir(const char *path, Filter f)
{
    struct dirent **namelist = NULL;
    int n = ::scandir(path, &namelist, f, alphasort);
    assert(namelist == NULL);
    return n;
}




pid_t Getpid()
{
    return ProcessPid;
}
std::string GetpidAsString()
{
    return std::to_string(Getpid());
}

uid_t Getuid()
{
    return getuid();
}
//获取主机名字
std::string GetHostName()
{
    char name[256] = { 0 };
    if(!gethostname(name, 256))
        return name;
    else
        return "";
}


//
string ReadStatusFile()
{
    File statusFile("/proc/self/status");
    std::string Status;
    size_t nread = statusFile.ReadFile(99999, Status);
    if(nread < 0)
        return "";
    else
        return Status;
}



//获取进程相关状态
int GetThreadTotalNum()
{
    string s = ReadStatusFile();
    int pos = s.find("Threads:");
    if(pos != string::npos)
    {
        return ::atoi(s.c_str() + pos + 8);
    }
    return -1;
}
std::vector<pid_t> GetAllThreadPid()
{
    vector<pid_t> pidList;
    ThreadPidList = &pidList;
    base::ProcessInfo::Scandir("/proc/self/task/", ThreadPidFilter);
    ThreadPidList = NULL;
    return pidList;
}
int GetOpendFileNum()
{
    FdNumber = 0;
    base::ProcessInfo::Scandir("/proc/self/fd/", FdNumberFilter);
    return FdNumber;
}
//获取可执行文件的路径在哪
std::string GetExecPath()
{
    char execpath[256] = { 0 };
    ::readlink("/proc/self/exe", execpath, 256);
    return execpath;
}


//获取进程运行频率、cpu时间以及进程启动时间

const double GetTicksPerSecond()
{
    return clickTicks;
}



CpuTime GetCpuTime()
{

    double usertime = 0.0;
    double systime  = 0.0;
    const double hz = GetTicksPerSecond();
    struct tms ticks;
    if(::times(&ticks) >= 0)
    {
        usertime = ticks.tms_utime / hz;
        systime  = ticks.tms_stime / hz;
    }
    return CpuTime(usertime, systime);
 
}
const TimeStamp GetStartTime()
{
    return startTime;
}






















}

}
