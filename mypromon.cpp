#include <iostream>
#include <sstream>
#include<string>
#include<deque>
#include"./nets/http/HttpServer.h"
#include "bases/TimeStamp.h"
#include "bases/ProcessInfo.h"
#include "nets/http/HttpRequest.h"
#include "nets/http/HttpRespone.h"

using namespace std;
using namespace net;
using namespace net::http;
using namespace base;


struct StatData
{
  void parse(const char* startAtState, int kbPerPage)
  {
    // istringstream is probably not the most efficient way to parse it,
    // see muduo-protorpc/examples/collect/ProcFs.cc for alternatives.
    std::stringstream iss(startAtState);

    //            0    1    2    3     4    5       6   7 8 9  11  13   15
    // 3770 (cat) R 3718 3770 3718 34818 3770 4202496 214 0 0 0 0 0 0 0 20
    // 16  18     19      20 21                   22      23      24              25
    //  0 1 0 298215 5750784 81 18446744073709551615 4194304 4242836 140736345340592
    //              26
    // 140736066274232 140575670169216 0 0 0 0 0 0 0 17 0 0 0 0 0 0

    iss >> state;
    iss >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags;
    iss >> minflt >> cminflt >> majflt >> cmajflt;
    iss >> utime >> stime >> cutime >> cstime;
    iss >> priority >> nice >> num_threads >> itrealvalue >> starttime;
    long vsize, rss;
    iss >> vsize >> rss >> rsslim;
    vsizeKb = vsize / 1024;
    rssKb = rss * kbPerPage;
  }
  // int pid;
  char state;
  int ppid;
  int pgrp;
  int session;
  int tty_nr;
  int tpgid;
  int flags;

  long minflt;
  long cminflt;
  long majflt;
  long cmajflt;

  long utime;
  long stime;
  long cutime;
  long cstime;

  long priority;
  long nice;
  long num_threads;
  long itrealvalue;
  long starttime;

  long vsizeKb;
  long rssKb;
  long rsslim;
};

class MyProcmon
{
public:
    MyProcmon();






    void OnRequest(HttpRequest &req, HttpRespone *resp)
    {
        resp->SetVersion('1');
        resp->SetResponeType(net::http::HttpRespone::kAccept);
        resp->SetHeader("Connection", "keep_alive");

        string url = req.GetUrl();

        if(url == "/")
        {

        }
        else if(url == "/cmdline")
        {

        }

    }




private:
    HttpServer server_;
    TimeStamp curTime_;
    TimeStamp startTime_;
    pid_t moniterPid_;      //被监控的程序pid
    string cmdline_;        //运行程序的命令
    string exeline_;
    string curQuery_;
    StatData stat_;         ///<进程状态
    deque<ProcessInfo::CpuTime> cpuTimeQueue_;


}


















int main(int argc, char** argv)
{
    if(argc < 3)
    {
        cout<<"Usage:./mypromon pid port";
        return -1;
    }


    std::cout << "Hello world" << std::endl;
    return 0;
}

