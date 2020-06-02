#pragma once
#include<memory>
#include"Callbacks.h"
#include "InetAddr.h"
#include"Socket.h"

/**
 * @file:TCP连接类
 * 需求：管理TCP连接，应该具有连接失败重连的功能
 *                  连6次，每次分别间隔1s，5s，20s，60s，120s，180s ///<其实这不太严谨！
 */

namespace net
{

class Channel;
class EventLoop;
class Connector
{
public:
    enum State { kBegin ,kConnecting/*处于Connecting状态*/, kConnectingOK/*已经conection结束，正处于TCPClient->NewConnection阶段*/,kConnected/*连接已建立*/, kRetry, kFail };
    const static int kRetryTimes = 6;
    Connector(EventLoop *loop, socket::InetAddr &peerAddr);
    ~Connector() {}

    void SetConnectOkCallBack(ConnectorConnectOKCb cb)      { okCb_   = std::move(cb);   }
    void SetConnectFailCallBack(ConnectorConnectFailCb cb)  { failCb_ = std::move(cb); }

    void Start();
    int  GetSockFd();
    State GetCurState()		{ return curState_; }

private:
    void Connect();      ///<仅限在定时器回调中使用
    void Retry(int sockfd);
    void Connecting(int sockfd);

    void ReConnectSetTimer();
    void SetState(State state)      { curState_ = state; }
    void ReConnect();           ///<定时器回调函数


private:
    EventLoop *loop_;
    //为了连接失败后的套接字能自动关闭，这里用智能指针管理
    int sockfd;       ///<如果连接失败，应该立马close,如果连接成功，该socket应转移到TcpConnecion，此处应失效

    const socket::InetAddr &peerAddr_;

    int connectRound_;      ///<重连次数
    bool isReConnecting_;   ///<是否在等待重连
    State curState_;        ///<当前连接状态

    //注意占位符
    int reconnectTime[kRetryTimes + 1] = { 0,1,5,5,5,5,5 };         ///<重连时间，按秒计算

    ConnectorConnectOKCb    okCb_;
    ConnectorConnectFailCb  failCb_;
};

}
