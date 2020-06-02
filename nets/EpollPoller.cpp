#include<map>
#include<vector>
#include<sys/epoll.h>
#include"EpollPoller.h"
#include"../bases/Logging.h"
using namespace base;
namespace net
{


EpollPoller::EpollPoller()
{
    epollFd_ = epoll_create(kEpollDefaultSize_);
    if(epollFd_ < 0)
        LOG_FATAL<<"EpollCreate fail";
}

EpollPoller::~EpollPoller()
{
    ::close(epollFd_);
}

void EpollPoller::Poll(std::vector<Channel*>& activeChannels)
{
    pollArray_.clear();
    if(pollChannel_.size() == 0)
    {
        LOG_TRACE<<"now epoll tree has no events...";
        return;
    }
    int nready = epoll_wait(epollFd_,&*pollArray_.begin(), pollChannel_.size(), kOneLoopTime_); ///<EPOLL主函数
    if(nready > 0)
        FillActiveChannels(nready, activeChannels);
    else if(nready == 0)
    {
        LOG_TRACE<<"this EventLoop round has no happening events";
    }
    else
    {
        LOG_ERROR<<"Epoll Error!";
    }
}

void EpollPoller::RemoveChannel(Channel *target)
{
    assert(target != NULL);
    assert(target->GetEvents() != 0);
    int targetFd = target->GetFd();
    assert(targetFd >= 0);
    std::map<int, Channel*>::iterator it = pollChannel_.find(targetFd);
    assert(it != pollChannel_.end());
    epoll_ctl(epollFd_, EPOLL_CTL_DEL, targetFd, NULL);
}

void EpollPoller::UpdateChannel(Channel *target, int targetEvent)
{
    assert(target != NULL);
    int targetFd = target->GetFd();
    assert(targetFd >= 0);
    std::map<int, Channel*>::iterator it = pollChannel_.find(targetFd);
    if(it == pollChannel_.end())    ///<说明是新的Channel
    {
        struct epoll_event epollfd;
        epollfd.events = targetEvent;
        epollfd.data.ptr = static_cast<void*>(target);
        int isOK = epoll_ctl(epollFd_, EPOLL_CTL_ADD, targetFd, &epollfd); ///<挂到监听树上
        if(isOK < 0)
        {
            LOG_ERROR<<"add new Channel to epoll tree fail!";
            return;
        }
        pollChannel_[targetFd] = target; ///<添加到map
    }
    else
    {
        assert(pollChannel_[targetFd]->GetFd() == targetFd);
        struct epoll_event epollfd;
        epollfd.events = targetEvent;
        epollfd.data.ptr = static_cast<void*>(target);
        int isOK = epoll_ctl(epollFd_, EPOLL_CTL_MOD, targetFd, &epollfd);
        if(isOK < 0)
        {
            LOG_ERROR<<"change Channel to epoll tree fail!";
            return;
        }
    }

}

void EpollPoller::FillActiveChannels(int nready, std::vector<Channel*>& activeChannels)
{
    assert(nready > 0);
    assert(pollArray_. size() == nready);
    for(auto activeEvent : pollArray_)
    {
        activeChannels.push_back(static_cast<Channel*>(activeEvent.data.ptr));
    }
}
const int EpollPoller::kEpollDefaultSize_ = 10;
const int EpollPoller::kOneLoopTime_ = 5000;
}
