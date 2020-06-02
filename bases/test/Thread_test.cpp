#include <iostream>
#include"Thread.h"
#include"ThreadPool.h"
#include"CurrentThread.h"
#include<functional>
#include<string>

void thread_1_func()
{
    std::cout<<"scuesss into thread_1_func"<<std::endl;
    std::cout<<base::CurrentThread::Tid()<<std::endl;
    std::cout<<base::CurrentThread::GetThreadName()<<std::endl;
    std::cout<<base::CurrentThread::GetThreadNameAsString()<<std::endl;
}



int main()
{
    base::Thread thread1(thread_1_func);

    base::ThreadPool mypool(thread_1_func, 5);

    mypool.start();
    
    //thread1.start();
    std::cout << "Hello world" << std::endl;
    return 0;
}

