

#include"Channel.h"
#include "EpollPoller.h"
#include"../bases/Logging.h"
#include"TcpConnection.h"
#include"EventLoop.h"
#include <cassert>
#include <memory>
#include <sys/poll.h>
using namespace base;
namespace net
{

Channel::Channel(EventLoop *loop, int fd)
        :fd_(fd),
         loop_(loop),
         tie_(),
         isTied_(false),
         events_(0),
         revents_(0),
         arrayIndex_(-1)
{
}
void Channel::HandleEvent()
{
    std::shared_ptr<TcpConnection> sharedTie = tie_.lock();
    if(sharedTie != NULL)
    {
        isTied_ = true;
        LOG_TRACE<<"now Channel weakptr tie TcpConnection...";
        HandleEventWithGuard();
        isTied_ = false;
    }
    else
        HandleEventWithGuard();
    SetRevents(0);
    LOG_TRACE<<"In Function:" <<__func__<<"over shared_ptr<TcpConnection>.use_count is "<<static_cast<int>(tie_.use_count());    
}

void Channel::HandleEventWithGuard()
{
    if(revents_ & POLLIN)
    {
        if(readCallBack_)
            readCallBack_();    ///<如果读没有设置回调，会出现什么情况？
        else
            { LOG_WARN<<"Channel:"<<fd_<<" occur POLLIN but has no set ReadCallback..."; }
    }
    else if(revents_ & POLLOUT)
    {

        if(writeCallBack_)
            writeCallBack_();    ///<如果读没有设置回调，会出现什么情况？
        else
            { LOG_WARN<<"Channel:"<<fd_<<" occur POLLOUT but has no set WriteCallback..."; }
    }
    else if(revents_ & POLLERR || revents_ & POLLHUP)
    {
        LOG_INFO<<"Channel POLLERR or POLLHUP occur";
        if(errorCallBack_)
            errorCallBack_();
    }
}

void Channel::EnableReading()
{
    events_ |= kReadEvents;
    assert(this != NULL);
    loop_->UpdateChannel(this,events_);
}
void Channel::EnableWriting()
{
    events_ |= kWriteEvents;
    assert(this != NULL);
    loop_->UpdateChannel(this,events_);
}
void Channel::DisableAll()
{
    events_ = kNoneEvents;
    assert(this != NULL);
    loop_->UpdateChannel(this,events_);
}

void Channel::DisableWriting()
{
    events_ &= ~kWriteEvents;
    assert(this != NULL);
    loop_->UpdateChannel(this,events_);
}

void Channel::RemoveChannel()
{
    assert(revents_ == 0);
    assert(events_ == 0);
    assert(this != NULL);
    loop_->RemoveChannel(this);
}


const int Channel::kReadEvents  = POLLIN | POLLPRI;  ///<POLLPRI的作用是什么？
const int Channel::kWriteEvents = POLLOUT;
const int Channel::kErrorEvents = POLLERR | POLLHUP | POLLRDHUP;
const int Channel::kNoneEvents  = 0;

}

