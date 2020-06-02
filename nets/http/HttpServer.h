#pragma once
#include"../TcpServer.h"
#include"../Callbacks.h"
#include"HttpRequest.h"
#include"HttpRespone.h"


namespace net
{
namespace http
{
class HttpServer
{
public:
    typedef std::function<void(const HttpRequest &req, HttpRespone *res)> HttpHandlerCallBack;

    HttpServer(EventLoop *loop, const char*ip = "127.0.0.1", const unsigned int port = 6667);
    ~HttpServer() {}

    void SetHandlerCallBack(HttpHandlerCallBack cb)
    {handlerCb_ = std::move(cb);}

    bool SetThreadNum(int threadNum);
    void Start();


private:
    void OnMessage(const TcpConnectionPtr &ptr, Buffer &buf, base::TimeStamp &recvTime);
    void OnConnection(const TcpConnectionPtr &ptr);

    void DoRequest(const TcpConnectionPtr&ptr, const HttpRequest &req);
    


private:
    EventLoop *loop_;
    TcpServer server_;
    int threadNum_;
    bool started_;


    HttpHandlerCallBack handlerCb_;



};








}


}
