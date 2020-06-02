#include <cassert>
#include <cstddef>
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include<string.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include<strings.h>
#include <time.h>
#include<unistd.h>
#include<stdlib.h>
#include<limits.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include<arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "nets/Channel.h"
#include "nets/EpollPoller.h"
#include"bases/Logging.h"
#include"nets/EventLoop.h"
using namespace base;
#define SERV_IP "127.1"
#define SERV_PORT 6666

/**
 * 测试目标：eventloop是否正确定义、定义一个Chanel能不能正确响应其回调
 */


void readcb(int fd)
{
    char buf[65536] = { 0 };
    ::read(fd, buf, sizeof buf);
    std::cout<<buf<<std::endl;

}

void* testInLoop(void *arg)
{
    net::EventLoop *loop = (net::EventLoop*)arg;
    assert(loop != NULL);
    LOG_TRACE<<"now begin loop";
    std::cout<<"hello"<<std::endl;
    loop->Loop();
    return NULL;
}

int main()
{
    net::EventLoop loop;
    pthread_t tid;
    pthread_create(&tid, NULL, testInLoop, (void*)&loop);
    
    int filefd = open("./a.out", O_RDWR);
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_length = sizeof(client_addr);
    bzero(&serv_addr, sizeof(serv_addr));
    bzero(&client_addr, sizeof(client_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);

    int listenfd = socket(AF_INET,SOCK_STREAM, 0);
    //fcntl(listenfd, F_SETFL, O_NONBLOCK);
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 0);

    while(1)
    {
        LOG_INFO<<"NOW BEGIN ACCEPT!";
        ::poll(NULL, 0, 5000);
        int fd = accept(listenfd, (struct sockaddr*)&client_addr, &client_length);
        net::Channel newChannel(&loop, fd);
        newChannel.SetReadCallBack(std::bind(readcb, fd));
        newChannel.EnableReading();
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

