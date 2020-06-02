#pragma once



#include<functional>
#include<memory>
#include"Callbacks.h"
/**
 * 作用：文件描述符控制类，一个Channel完全控制一个fd但不掌管其生命周期（即Channel析构时不close）
 */
namespace net
{
class EventLoop; 
class TcpConnection;

class Channel
{
public:
    Channel(EventLoop *loop, int fd);
    ~Channel() = default;

    void HandleEvent();

    void EnableReading();
    void EnableWriting();
    void DisableAll();
    void DisableWriting();

    void SetReadCallBack(ChannelCallBack cb)            { readCallBack_ = std::move(cb); }
    void SetWriteCallBack(ChannelCallBack cb)           { writeCallBack_ = std::move(cb); }
    void SetErrorCallBack(ChannelCallBack cb)           { errorCallBack_ = std::move(cb);}
    //for only EventLoop
    void SetArrayIndex(int arrayIndex)  { arrayIndex_ = arrayIndex; }
    void SetRevents(int revents)        { revents_ = revents; }
    void SetTiedPtr(const TcpConnectionPtr &ptr) { tie_ = ptr; }

    int  GetArrayIndex() const          { return arrayIndex_; }
    int  GetRevents() const             { return revents_; }
    int  GetEvents() const              { return events_; }
    int  GetFd() const                  { return fd_; }
    bool GetIsTied() const              { return isTied_; }
    std::weak_ptr<TcpConnection> GetTiedPtr() { return tie_; }
    EventLoop* GetLoop() const          { return loop_; }

    void RemoveChannel();
private:
    void HandleEventWithGuard();
private:
    int fd_;
    EventLoop *loop_;

    short events_;
    short revents_;

    ChannelCallBack readCallBack_;
    ChannelCallBack writeCallBack_;
    ChannelCallBack errorCallBack_;

    int arrayIndex_;

    const static int kReadEvents;
    const static int kWriteEvents;
    const static int kErrorEvents;
    const static int kNoneEvents;

    std::weak_ptr<TcpConnection> tie_;      ///<用于延长TCPConnection的生命周期
    bool isTied_;

};

}
