#pragma once
#include<pthread.h>
#include"NonCopyable.h"
namespace base
{

class MutexLock: base::NonCopyable
{
public:
    MutexLock()
    {
        pthread_mutex_init(&mutex_, NULL);
    }
    ~MutexLock()
    {
        pthread_mutex_destroy(&mutex_);
    }
private:
    friend class MutexLockGuard;
    friend class Condition;
    void Lock()
    {
        pthread_mutex_lock(&mutex_);
    }
    void UnLock()
    {
        pthread_mutex_unlock(&mutex_);
    }
    pthread_mutex_t& get()
    {
        return mutex_;
    }
    pthread_mutex_t  mutex_;
};


}
