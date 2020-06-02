#include"HttpServer.h"
#include "HttpRespone.h"
#include"HttpRequest.h"
#include"AnalyzeHttp.h"
#include <ostream>

using namespace std::placeholders;
using namespace base;


namespace net
{
namespace http
{


HttpServer::HttpServer(EventLoop *loop, const char*ip, const unsigned int port)
    :loop_(loop),
     server_(loop, ip, port),
     threadNum_(0),
     started_(false)
    {
        server_.SetReadCallback(std::bind(&HttpServer::OnMessage, this, _1, _2, _3));
        server_.SetOnConnectCallback(std::bind(&HttpServer::OnConnection, this, _1));
    }




bool HttpServer::SetThreadNum(int threadNum)
{
    if(threadNum < 0)
        return false;
    if(!started_)
    {
        threadNum_ = threadNum;
    }
    return true;
}


void HttpServer::Start()
{
    started_ = true;
    server_.SetThreadNum(threadNum_);
    LOG_INFO<<"HttpServer start....Thread Number is "<<threadNum_;
    loop_->Loop();
}

void HttpServer::OnMessage(const TcpConnectionPtr &ptr, Buffer &buf, base::TimeStamp &recvTime)
{
    LOG_DEBUG<<"Message Come...";
    AnalyzeHttp *analy = static_cast<AnalyzeHttp*>(ptr->GetContext());
    
    if(analy == NULL || analy->Analyze(buf, recvTime) == false)
    {
        ptr->Send("HTTP/1.1 400 Bad Request\r\n\r\n");
        ptr->Shutdown();
    }
    if(analy->GotAllData())
    {
        DoRequest(ptr, analy->GetHttpReq());
        analy->reset();
    }

}
void HttpServer::OnConnection(const TcpConnectionPtr &ptr)
{
    if(ptr->Connected())
    {
        AnalyzeHttp *analy = new AnalyzeHttp();
        ptr->SetContext(analy);
    }
    else if(ptr->Disconnected())
    {
        delete static_cast<AnalyzeHttp*>(ptr->GetContext());
    }
}
void HttpServer::DoRequest(const TcpConnectionPtr &ptr,const HttpRequest &req)
{
    bool close = req.GetHeader("Connection") == " close" ||
        req.GetHeader("Connection") == "Close" ||
        req.GetVersion() == HttpRequest::kHttp10 ? true : false ;
    HttpRespone res;
    handlerCb_(req, &res);

    std::string resString = res.AppendToString();

    //写上response转化函数后，将其发送出去
    ptr->Send(resString);

    if(close)
    {
        ptr->Shutdown();
        LOG_DEBUG<<"HttpServer write Shutdown !!!!";
    }

}









}   ///<END OF namespace http
}   ///<END OF namespace net
