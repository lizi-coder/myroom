#include <functional>
#include <iostream>
#include"SingleTon.h"
#include"ThreadPool.h"
#include<sys/syscall.h>
#include<unistd.h>
#include<poll.h>
/**
  * 测试内容:Thread类、ThreadPool类、懒汉单例模式SingleTon
  * @date 2020/05/04
  *
  */


void thread_func()
{
    std::cout<<"now thread func run ok! pid is "<<syscall(SYS_gettid)<<std::endl;
    ::poll(NULL, 0, rand()%7000);
    std::cout<<"now thread func OVER ok! pid is "<<syscall(SYS_gettid)<<std::endl;;

}



int main()
{
    std::function<void()> func = std::bind(thread_func);
    unsigned int num = 10;
    base::ThreadPool* testPool = base::SingleTon<base::ThreadPool, std::function<void()>, unsigned int>::pointer(func,num);
    testPool->start();
    ::poll(NULL, 0, 10000);
    base::ThreadPool* testPool2 = base::SingleTon<base::ThreadPool, std::function<void()>, unsigned int>::pointer(func,num);
    if(testPool == testPool2)
        std::cout<<"equal"<<std::endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}

