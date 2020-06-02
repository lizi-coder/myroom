#pragma once

#include "HttpRequest.h"
#include"../Buffer.h"



namespace net
{

namespace http
{



class AnalyzeHttp
{
public:
    enum CurStatus { kInit, kAnalyseReqLine, kAnalyseHeaders, kGotAll };
    AnalyzeHttp()
	:status_(kInit)
 {}
    ~AnalyzeHttp() {}

    void reset();
    bool Analyze(Buffer &buf, base::TimeStamp &recvTime);
    bool GotAllData()   { return status_ == kGotAll; }

    const HttpRequest& GetHttpReq() {return req;}

private:
    bool ParseRequestLine(const char*, const char*);
    void SetStatus(CurStatus curStatus )
    {status_ = curStatus;}

private:
    HttpRequest req;
    CurStatus status_;

};



}

}
