#include<pthread.h>
#include"Logging.h"
using namespace base;


void* thread1(void *arg)
{
    LOG_INFO<<"thread1!";
    LOG_DEBUG<<"thread1 debug";
    return NULL;
}



int main()
{

    LOG_INFO<<"HELLO";
    errno = 3;
    LOG_ERROR<<"error!!!";
    LOG_ERROR<<"error!!!";
    LOG_WARN<<"warn";
    LOG_DEBUG<<"debug";
    LOG_TRACE<<"trace";
   // LOG_FATAL<<"FATAL!!!!!!!!";

    pthread_t tid;

    pthread_create(&tid, NULL, thread1, NULL);

    pthread_join(tid, NULL);
    return 0;
}

