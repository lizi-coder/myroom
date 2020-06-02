#pragma once

#include "Poller.h"
#include"Channel.h"
#include"../bases/MutexLock.h"
#include<vector>
#include<map>
#include<poll.h>
namespace net
{

class PollPoller : public Poller
{
public:
    PollPoller();
    ~PollPoller();

    void Poll(std::vector<Channel*>& activeChannels) override;
    void RemoveChannel(Channel *target) override;
    void UpdateChannel(Channel *target, int events) override;
    
private:
    void FillActiveChannels(int nready, std::vector<Channel*>& activeChannels);
private:
    base::MutexLock mutex_;
    std::vector<struct pollfd> pollArray_;  ///<poll的监听对象
    std::map<int, Channel*> pollChannel_;   ///<fd和Channel的映射
    const static int kOneEventLoopTime_;             ///<5000ms循环一次
};

}
