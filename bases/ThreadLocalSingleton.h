#ifndef _PTHREAD_LOCAL_SINGALTON_
#define _PTHREAD_LOCAL_SINGALTON_
#include<pthread.h>
#include<assert.h>

/**
 * @file ThreadLocalSingleton.h
 * 功   能 ： 这是线程局部单例模式！
 * 设计要点：线程局部删除器deleter设计、pthread_key_*系列函数使用
 * 
 */


namespace base
{
template<class T>
class ThreadLocalSingleton
{
public:
    ThreadLocalSingleton() = delete;
    ThreadLocalSingleton(const T& singleton) = delete;
    static T& instance()   ///<获取实例
    {
        if(singleton_ == NULL)
        {
            singleton_ = new T();
            deleter_.setValue(singleton_);
        }
        return *singleton_;
    }
    /**
     * 这个是获取指针！
     */

    static T* pointer()
    {
        return singleton_;
    }

private:
    T& operator=(const T& singleton);
    static void  dstructor(void *arg)
    {
        if(arg)
            delete static_cast<T*>(arg);
    }
private:
    class Deleter
    {
    public:
        Deleter()  {pthread_key_create(&key_, &dstructor);}
        ~Deleter() {pthread_key_delete(key_);}

        void setValue(T *localValue)
        {
            assert(pthread_getspecific(key_) == NULL);
            pthread_setspecific(key_, localValue);
        }
    private:
        pthread_key_t key_;
    };
private:
    __thread static  T* singleton_;
    static Deleter deleter_;
};

template<class T>
__thread T* ThreadLocalSingleton<T>::singleton_ = NULL;

template<typename T>
//这里为什么需要一个typename？
typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::deleter_;

}
#endif


