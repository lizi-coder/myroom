#include"ThreadPool.h"
#include "CountDownLatch.h"
#include <memory>

namespace base
{


ThreadPool::ThreadPool(ThreadFunc threadFunc_, int threadTotalNum)
{
    std::string poolBaseName = "ThreadPool@Thread0";
    for(int num = 0; num < threadTotalNum; num++ )
    {
        ThreadFunc func = threadFunc_;
        poolBaseName[poolBaseName.size()-1] = '0' + num + 1;
        std::unique_ptr<Thread> threadPtr(new Thread(func, poolBaseName));
        //threadPtrArray.push_back(threadPtr);    ///<当unique_ptr压入vector时，会发生new，这么写是错误的！！
        threadPtrArray.emplace_back(threadPtr.release());    ///<知识点：用emplace来构造而不是拷贝！
    }
}
ThreadPool::~ThreadPool()
{

}
void ThreadPool::start()
{
    for(auto& threadPtr : threadPtrArray)
    {
        threadPtr->start();
    }
}
} // END OF base namespace
