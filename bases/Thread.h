#pragma once
#include "CurrentThread.h"
#include"NonCopyable.h"
#include"MutexLock.h"
#include"MutexLockGuard.h"
#include"Condition.h"
#include<functional>
#include"CountDownLatch.h"
namespace base
{

class Thread
{
public:
    typedef std::function<void()> ThreadFunc;
    Thread(ThreadFunc threadFunc, std::string& name, int createNum = 1);///<问题：普通函数转为ThreadFunc失败？？
    Thread(ThreadFunc threadFunc, int createNum = 1);
    ~Thread(); 
    void start();
    std::string& GetThreadName()
    {
        return name_;
    }
    bool SetThreadName(std::string &name)  ///<设置线程名,格式为name@tid
    {
        assert(name.size() != 0);
        if(tid_ != CurrentThread::Tid())
            return false;
        if(isStarted_)
        {
            name_ = name;
            CurrentThread::SetThreadName(name_);
            return true;
        }
        return false;
    }
private:
    static void* runInThread(void *arg);        ///<线程入口函数
private:
    struct ThreadData
    {
    public:
        ThreadData(Thread *thread,ThreadFunc &threadFunc, pid_t &tid, MutexLock &mutex, CountDownLatch &latch, bool isStarted = false);
        ~ThreadData();
        
        void trueRunThread();

    private:
        Thread *thread_;
        ThreadFunc threadFunc_;
        CountDownLatch &latch_;
        bool isStarted_;
    };

private:
    ThreadFunc threadFunc_;
    std::string name_;              ///<线程名字
    pid_t tid_;
    pthread_t pthreadID_;
    unsigned int threadID_;         ///<数字类型的ID，从1,2,3....
    bool isStarted_;            ///<线程是否已经start
//
    MutexLock mutex_;
    CountDownLatch latch_;


    static unsigned int threadNum_;  ///<事实上这里需要原子操作
};


}
