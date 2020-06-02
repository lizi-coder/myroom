#include <iostream>
#include"ThreadLocalSingleton.h"
#include<sys/types.h>
#include<unistd.h>
#include<sys/syscall.h>

struct Mytest
{
public:
    //Mytest(pid_t threadPid): threadPid_(threadPid) { std::cout<<"create Mytest! threadPid is "<<threadPid<<std::endl; }
    Mytest() {
        threadPid_ = syscall(SYS_gettid);
        std::cout<<"create Mytest! threadPid is "<<threadPid_<<std::endl; 
    }

private:
        pid_t threadPid_;
};


void* thread_1(void *arg)
{
    Mytest test1 = base::ThreadLocalSingleton<Mytest>::instance();
    Mytest test2 = base::ThreadLocalSingleton<Mytest>::instance();
    Mytest test3 = base::ThreadLocalSingleton<Mytest>::instance();

    return NULL;

}

void* thread_2(void *arg)
{
    Mytest test1 = base::ThreadLocalSingleton<Mytest>::instance();
    Mytest test2 = base::ThreadLocalSingleton<Mytest>::instance();
    Mytest test3 = base::ThreadLocalSingleton<Mytest>::instance();
    return NULL;
}



int main()
{
    Mytest test1 = base::ThreadLocalSingleton<Mytest>::instance();
    Mytest test2 = base::ThreadLocalSingleton<Mytest>::instance();
    Mytest test3 = base::ThreadLocalSingleton<Mytest>::instance();
    pthread_t th1, th2;
    pthread_create(&th1, NULL, thread_1, NULL);
    pthread_create(&th2, NULL, thread_2, NULL);
    std::cout << "Hello world" << std::endl;
    sleep(1);
    return 0;
}

