#pragma once
#include "MutexLock.h"
#include"NonCopyable.h"
#include<pthread.h>
namespace base
{

class Condition : NonCopyable
{
public:
    Condition(MutexLock &mutex)
        :mutex_(mutex)

    {
        pthread_cond_init(&cond_,NULL);
    }
    ~Condition()
    {
        pthread_cond_destroy(&cond_);
    }

    void wait()
    {
        pthread_cond_wait(&cond_,&mutex_.get());
    }
    void notify()
    {
        pthread_cond_signal(&cond_);
    }
    void broadcast()
    {
        pthread_cond_broadcast(&cond_);
    }
    

private:
    pthread_cond_t cond_;
    MutexLock& mutex_;

};


}
