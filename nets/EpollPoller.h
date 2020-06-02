#pragma once

#include<vector>
#include<map>
#include "Poller.h"
#include"Channel.h"
namespace net
{

class EpollPoller : public Poller
{
public:
    EpollPoller();
    ~EpollPoller(); 

    void Poll(std::vector<Channel*>& activeChannels) override;
    void RemoveChannel(Channel *target) override;
    void UpdateChannel(Channel *target, int events) override;
private:
    void FillActiveChannels(int nready, std::vector<Channel*>& activeChannels);
    int epollFd_;
    std::vector<struct epoll_event> pollArray_;  ///<epoll的监听对象
    std::map<int, Channel*> pollChannel_;       ///<epoll的监听对象的Channel
    const static int kOneEventLoopTime_;             ///<5000ms循环一次

    static const int kEpollDefaultSize_;
    static const int kOneLoopTime_;

};

}
