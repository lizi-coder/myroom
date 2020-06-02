#pragma once
#include"MutexLock.h"
#include"NonCopyable.h"
namespace base
{

class MutexLockGuard : base::NonCopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex): mutex_(mutex)
    {
        mutex_.Lock();
    }
    ~MutexLockGuard()
    {
        mutex_.UnLock();
    }
private:
    MutexLock &mutex_;

};

}
