#include <sys/poll.h>
#include <vector>
#include<poll.h>
#include<map>
#include"Channel.h"
#include"../bases/Logging.h"
#include"PollPoller.h"
#include"../bases/MutexLockGuard.h"
using namespace base;
namespace net
{
const int PollPoller::kOneEventLoopTime_ = 5000;
PollPoller::PollPoller()
{

}
PollPoller::~PollPoller()
{

}
void PollPoller::Poll(std::vector<Channel*>& activeChannels)
{
    int nready = ::poll(&pollArray_[0], pollArray_.size(), kOneEventLoopTime_);
    if(nready > 0)
    {
        FillActiveChannels(nready, activeChannels);
    }
    else if(nready == 0)
    {
        //LOG_TRACE<<"EventLoop nothing happens";
    }
    else
    {
        LOG_ERROR<<"EventLoop error occur!";
    }
}

void PollPoller::RemoveChannel(Channel *target) ///<删除策略：和最后一个元素互换后再把最后一个弹出去
{
    LOG_DEBUG<<"Begin RemoveChannel...";
    assert(target != NULL);
    assert(target->GetEvents() == 0);

    int vectorEnd   = pollArray_.size() -1;
    int deleteIndex = target->GetArrayIndex();
    if(vectorEnd == deleteIndex)
    {
        pollChannel_.erase(target->GetFd());
        pollArray_.pop_back();
        return;
    }
    else if(vectorEnd < deleteIndex)
    {
        LOG_FATAL<<"VectorEnd < deleteIndex !!"<<"end is "<<vectorEnd<<", deleteIndex is "<<deleteIndex;
        return;
    }
    LOG_DEBUG<<"VectorEnd="<<vectorEnd<<",deleteIndex="<<deleteIndex;
    assert(vectorEnd >= deleteIndex);

    std::swap(pollArray_[vectorEnd], pollArray_[deleteIndex]);
    pollArray_.pop_back();
    pollChannel_.erase(target->GetFd());        ///<删除map<fd，Chanel*>对应的元素 
    assert(pollArray_.size() == pollChannel_.size());
    LOG_TRACE<<"After RemoveChannel, pollArray has "<<static_cast<int>(pollArray_.size())
        <<" element, pollChannel has "<<static_cast<int>(pollChannel_.size())<<" elements";
    if(vectorEnd == deleteIndex || pollArray_.size() == 0)
        return;

    auto backEndElement = pollChannel_.find(pollArray_[deleteIndex].fd);    ///<此时的deleteIndex已经被原先的最后一个元素替换上了
    assert(backEndElement != pollChannel_.end());
    pollChannel_[pollArray_[deleteIndex].fd]->SetArrayIndex(deleteIndex);
}

void PollPoller::UpdateChannel(Channel *target, int targetEvent)
{
    assert(target != NULL);
    int targetFd = target->GetFd();
    assert(targetFd >= 0);
    std::map<int, Channel*>::iterator it = pollChannel_.find(targetFd);
    if(it == pollChannel_.end())    ///<说明是新的Channel
    {
        struct pollfd pollfd;
        pollfd.fd = targetFd;
        pollfd.events  = targetEvent;
        pollfd.revents = 0;

        pollChannel_[targetFd] = target; ///<添加到map
        pollArray_.push_back(pollfd);    ///<添加到vector
        target->SetArrayIndex(pollArray_.size() - 1);
        LOG_DEBUG<<"Now fd "<<target->GetFd()<<" set Array index:"<<target->GetArrayIndex();

//        LOG_TRACE<<"new Channel comes!";
    }
    else
    {
        int targetIndex = target->GetArrayIndex();
        assert(targetIndex >= 0 && targetIndex < pollArray_.size());
        assert(pollArray_[targetIndex].fd == targetFd);
        pollArray_[targetIndex].events = targetEvent;
        LOG_TRACE<<"Channel modify! FD = "<<target->GetFd()<<"targetEvent is "<<targetEvent;
    }
}




void PollPoller::FillActiveChannels(int nready, std::vector<Channel*>& activeChannels)
{
    for(int round = 0; round <pollArray_.size() && nready > 0; round++)
    {
        if(pollArray_[round].revents != 0)        ///<忘记了怎么判断活跃事件？
        {
            
            std::map<int, Channel*>::iterator it = pollChannel_.find(pollArray_[round].fd);
            assert(it != pollChannel_.end());
            Channel *currentActiveChannel = (*it).second;
            currentActiveChannel->SetRevents(pollArray_[round].revents);
            activeChannels.push_back(currentActiveChannel);
            nready--;
        }
    }
}

}
