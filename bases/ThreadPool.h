#pragma once
#include"Thread.h"
#include"MutexLock.h"
#include"MutexLockGuard.h"
#include"CountDownLatch.h"
#include"NonCopyable.h"
#include<vector>
#include<memory>
/**
 * 功能：简易线程池，线程池中所有线程都调用threadFunc方法,需要保证其单例
 * 难点：如何做到start后等num个线程全部进入函数后再返回？
 */


namespace base
{


class ThreadPool: public NonCopyable
{
public:
    typedef std::function<void()> ThreadFunc;
    ThreadPool(ThreadFunc threadFunc_, int threadTotalNum);
    ~ThreadPool();
    void start();

    static void trueRunThread();
private:
private:
    ThreadFunc threadFunc_;
    int threadTotalNum_;
    std::vector<std::unique_ptr<Thread>> threadPtrArray;
};


}
