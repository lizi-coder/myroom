
#include"CurrentThread.h"
namespace base
{
    __thread pid_t CurrentThread::currentThreadTid_ = 0; 
    __thread char* CurrentThread::currentThreadName_= NULL; 
}

