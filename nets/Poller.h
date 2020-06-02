#pragma once
#include<vector>
#include"../bases/Noncopyable.h"
#include"Channel.h"
namespace net
{

class Poller : base::NonCopyable
{
public:
    Poller() {}
    virtual ~Poller() {}

    virtual void Poll(std::vector<Channel*>& activeChannel) = 0;
    virtual void RemoveChannel(Channel *target) = 0;
    virtual void UpdateChannel(Channel *target, int events) = 0;
private:
};

}
