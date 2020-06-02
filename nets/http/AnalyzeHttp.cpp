#include"AnalyzeHttp.h"
#include "HttpRequest.h"
#include"../Buffer.h"
#include"../../bases/Logging.h"
#include <algorithm>

using namespace base;

namespace net
{
namespace http
{


void AnalyzeHttp::reset()
{
    HttpRequest dummy;
    std::swap(dummy, req);
    SetStatus(kInit);
    LOG_DEBUG<<"Clear HttpRequest scuess!";
}

bool AnalyzeHttp::Analyze(Buffer &buf, base::TimeStamp &recvTime)
{
    bool done = true;
    bool analyzeAll = false;

    const char *crlf = buf.FindCRLF();
    if(crlf != NULL)
    {
        while(!analyzeAll)
        {
            LOG_TRACE<<"Into Analyze Loop, NOW status is "<<status_;
            const char *crlf = buf.FindCRLF();
            if(status_ == kInit || status_ == kAnalyseReqLine)
            {
                if(ParseRequestLine(buf.Peek(), crlf) == false)
                {
                    LOG_ERROR<<"Analyze Request Line error!";
                    done = false;
                    break;
                }
                else
                {
                    buf.RetrieveUntilCRLF();
                    SetStatus(kAnalyseHeaders);
                }
            }
            else if(status_ == kAnalyseHeaders)
            {
                const char *middle = std::find(buf.Peek(), buf.Peek() + buf.DataNumber(), ':');
                if(middle == NULL)  //没找到冒号说明数据没收齐
                    break;
                if(crlf == NULL)
                {
                    LOG_TRACE<<"GET ALL data OK!";
                    SetStatus(kGotAll);
                    buf.RetrieveUntilCRLF();
                    break;
                }
                req.AddHeader(buf.Peek(), middle, crlf);
                buf.RetrieveUntilCRLF();
            }
        }

    }
    return done;
}

bool AnalyzeHttp::ParseRequestLine(const char* begin, const char* end)
{
    LOG_DEBUG<<"\n"<<begin;
    bool ok = true;
    const char *start  = begin; 
    const char *finish = std::find(begin, end, ' ');    ///寻找第一个空格
    if(finish == end)
        return  false;
    req.SetReqType(start, finish);

    start = finish + 1;
    finish = std::find(start, end, ' ');    ///寻找第2个空格
    if(finish == end)
        return  false;
    req.SetReqURL(start, finish);

    start = finish + 1;
    std::string version(start, end);
    LOG_TRACE<<"version size:"<<version.size()<<" ,context:"<<version;
    if(version.size() != 8 || !std::equal(version.begin(), version.end() - 1, "HTTP/1."))
    {
        LOG_DEBUG<<"HTTP version Analyze error";
        return  false;
    }

    if(req.SetVersion(version[7]) == false)
        return false;

    return true;

}

}
}
