#pragma once

#include"NonCopyable.h"
#include"MutexLockGuard.h"
#include"MutexLock.h"
#include"Condition.h"

/**
 *
 *  疑问：它的具体作用是什么？用notify还是broadcast？会分析虚假唤醒吗？
 */
namespace base
{

class CountDownLatch
{
public:
    CountDownLatch(MutexLock &mutex, unsigned int count)
        :mutex_(mutex),
         cond_(mutex_),
         count_(count)
    {

    }
    ~CountDownLatch()
    {

    }

    void wait()
    {
        //想一下为什么要用while？虚假唤醒要会分析
        while(count_ > 0)
            cond_.wait();
    }

    void countDown()
    {
        MutexLockGuard lock(mutex_);
        if(--count_ == 0)
            cond_.broadcast();
    }
private:
    MutexLock &mutex_;
    Condition cond_;

    unsigned int count_;//应该是原子操作,这里用锁替代


};


}

