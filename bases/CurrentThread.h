#pragma once
#include<string>
#include<unistd.h>
#include<sys/syscall.h>
#include<assert.h>
namespace base
{

class CurrentThread
{
public:
    CurrentThread() {}
    ~CurrentThread() {}
    
    static pid_t Tid()
    {
        if(currentThreadTid_ == 0)
            currentThreadTid_ = syscall(SYS_gettid);
        return currentThreadTid_;
    }
    static const std::string GetThreadNameAsString()
    {
	assert(currentThreadName_ != NULL);
        return std::string(currentThreadName_);
    }
    static const char* GetThreadName()
    {
	assert(currentThreadName_ != NULL);
        return currentThreadName_;
    }
    static void SetThreadName(std::string &currentThreadName)
    {
	assert(currentThreadName.size() != 0);
        currentThreadName_ = const_cast<char*>(currentThreadName.c_str());
    }
    static void SetThreadName(char* currentThreadName)
    {
        assert(currentThreadName != NULL);
        currentThreadName_ = currentThreadName;
    }

private:
    static __thread pid_t currentThreadTid_;
    static __thread char* currentThreadName_;
    ///<注意：初始化位置在Thread::ThreadData::trueRunThread上
};


}
