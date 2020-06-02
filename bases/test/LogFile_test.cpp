#include <functional>
#include <iostream>
#include<memory>
#include"Logging.h"
#include"LogFile.h"
using namespace base;
LogFile file_("test_name");

void myappend(const char* msg, size_t length)
{
    file_.Append(msg,length);
}

void myflush()
{
    file_.Flush();
}

/**
 * LogFile:
 * 测试项目:1.写日志
 *          2.日志是否滚动成功
 *          3.滚动时间内已经写满了count个的处理方式
 *          4.死循环：超过rollsize的处理方法
 *
 *
 */

int main()
{
    Logging::SetOutPut(std::bind(&myappend, std::placeholders::_1, std::placeholders::_2));
    Logging::SetFlush(myflush);
    Logging::SetLogRank("INFO");
    int count = 1025;
    while(count > 0)
    {
        LOG_INFO<<"this is info";
        LOG_DEBUG<<"debug test";
        count--;
    }
    sleep(3);

    count = 1025;
    while(count > 0)
    {
        LOG_INFO<<"this is info";
        count--;
    }

    return 0;
}

