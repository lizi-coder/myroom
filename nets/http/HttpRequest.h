#pragma once
#include<string>
#include<map>
#include"../../bases/TimeStamp.h"

/**
 * HttpRequest类，用于存储HTTP请求的数据
 */

namespace net
{
namespace http
{



class HttpRequest
{
public:
    enum Version { kInValid, kHttp10, kHttp11 };
    enum ReqType { kUnknowType, kGET, kPOST };
    typedef std::string HeadName;
    typedef std::string HeadValue;
    
    bool SetVersion(const char versionFlag);
    bool SetReqType(const char* begin, const char *end);
    bool AddHeader(const char*begin, const char* middle, const char* end);

    void SetReqURL(const char *begin, const char* end)
    {reqURL_ = std::string(begin, end);}

    void SetRecvTime(base::TimeStamp &recvTime)     
    {recvTime_ = recvTime;}

    void SetQuery(const char *begin, const char* end);

    const std::string& GetHeader(std::string headname) const
    {
        auto it = headers_.find(headname);
        if(it == headers_.end())
            return nullString;
        else
            return it->second;
    }
    const std::string& GetQuery() const
    {return query_;}

    const std::string& GetUrl() const
    {return reqURL_;}

    const Version GetVersion() const {return version_;}
    HttpRequest()
        :Method_(kUnknowType),
         version_(kInValid),
         recvTime_(base::TimeStamp::InValid())
    {}
    ~HttpRequest() {}

private:
    ReqType      Method_;
    std::string  reqURL_;
    Version      version_;
    std::map<HeadName, HeadValue> headers_;
    std::string  body_;         ///<请求的body一般为空
    std::string query_;
    base::TimeStamp recvTime_;
    const static std::string nullString ;

};

}

}

