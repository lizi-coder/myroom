#include"Thread.h"
#include<assert.h>
#include <pthread.h>
#include <string>
#include"CurrentThread.h"
#include"Logging.h"

namespace base
{
    Thread::Thread(ThreadFunc threadFunc, std::string& name, int createNum)
        //:threadFunc_(std::move(threadFunc)),
        :threadFunc_(threadFunc),
        name_(name),
        tid_(0),
        mutex_(),
        threadID_(threadNum_),
        isStarted_(false),
        latch_(mutex_,1)
    {
        threadNum_ += 1;
    }

    Thread::Thread(ThreadFunc threadFunc, int createNum)
        //:threadFunc_(std::move(threadFunc)),
        :threadFunc_(threadFunc),
        name_("ThreadDefaultName"),
        tid_(0),
        mutex_(),
        threadID_(threadNum_),
        isStarted_(false),
        latch_(mutex_,1)
    {
        threadNum_ += 1;
    }

    Thread:: ~Thread()  ///<疑问：在没有start就析构了，会出现什么情况？
    {
        if(isStarted_)
            pthread_detach(pthreadID_);
    }
    
    void Thread::start()
    {
        if(tid_ == 0)
        {
            isStarted_ = true;
            Thread::ThreadData *data = new Thread::ThreadData(this,threadFunc_, tid_, mutex_, latch_, isStarted_);
            assert(data != NULL);
            pthread_create(&pthreadID_, NULL, Thread::runInThread, data);
            latch_.wait();
        }
    }

    void* Thread::runInThread(void *arg)
    {
        Thread::ThreadData *data = static_cast<ThreadData*>(arg);
        data->trueRunThread();
        delete data;
        return NULL;
    }

    
/**
 * 以下是ThreadData
 */
    Thread::ThreadData::ThreadData(Thread* thread,ThreadFunc &threadFunc, pid_t &tid, MutexLock &mutex, CountDownLatch &latch, bool isStarted)
        :thread_(thread),
         threadFunc_(std::move(threadFunc)),
         latch_(latch),
         isStarted_(isStarted)
    {

    }
    Thread::ThreadData::~ThreadData()
    {
    }
    void Thread::ThreadData::trueRunThread()
    {
        if(isStarted_)
        {
            //调用到它之后，进行解锁，start函数退出
            latch_.countDown();     ///<引用计数从1---> 0
            assert(thread_->GetThreadName().size() != 0);
            thread_->tid_ = CurrentThread::Tid();
            std::string name = thread_->GetThreadName() +", Tid:" + std::to_string(thread_->tid_);
            if(!thread_->SetThreadName(name))
                LOG_INFO<<"SetThreadName in Different Thread!";
            threadFunc_();
        }
    }
    unsigned int Thread::threadNum_ = 0;
}

