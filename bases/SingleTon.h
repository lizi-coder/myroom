#pragma once
#include "MutexLock.h"
#include "MutexLockGuard.h"
#include"NonCopyable.h"
#include<memory>
#include <utility>
#include"ThreadPool.h"

/**
 * 功能：实现懒汉单例模式，其中构造函数是带参的需要用可变模板参数
 * 难点：可变参数模板怎么写、完美转发怎么写、懒汉的双判断锁怎么利用
 */


namespace base
{
template<typename T, typename... Args>
class SingleTon:NonCopyable
{
public:
    static T& instance(Args&... arg)
    {
        
        if(pointer_ == NULL)
        {
            {
                MutexLockGuard mylock(mutex_);
                if(pointer_ == NULL)
                {
                    pointer_.reset((new T(arg...)));    
                }
            }
        }
        return *pointer_;
    }
    
    static T* pointer(Args&... arg)
    {
        return &instance(arg...);
    }

private:
    SingleTon() {}
    ~SingleTon() {}
    static void  destructor(T *arg)
    {
        if(arg)
            delete arg;
    }

private:
    static std::shared_ptr<T> pointer_;
    static MutexLock  mutex_;
};

template<typename T, typename... Args>
std::shared_ptr<T> SingleTon<T,Args...>::pointer_ = NULL;

template<typename T, typename... Args>
MutexLock SingleTon<T,Args...>::mutex_;
}










