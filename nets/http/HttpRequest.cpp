#include<assert.h>
#include"HttpRequest.h"

using namespace std;

namespace net
{
namespace http
{

bool HttpRequest::SetVersion(const char versionFlag)
{
    if(versionFlag == '1')
        version_ = kHttp11;
    else if(versionFlag == '0')
        version_ = kHttp10;

    return version_ != kInValid;
    
}

bool HttpRequest::SetReqType(const char* begin, const char *end)
{
    string type(begin, end);
    if(type == "GET")
        Method_ = kGET;
    else if(type == "POST")
        Method_ = kPOST;

    return Method_ != kUnknowType;
}

bool HttpRequest::AddHeader(const char*begin, const char* middle, const char* end)
{
    while(isspace(*begin))
        begin++;
    string headName(begin, middle);
    middle++;
    while(isspace(*middle))
        middle++;
    string headValue(middle, end);
    while(isspace(headValue[headValue.size() - 1]))
        headValue.erase(headValue.end() - 1);

    headers_[headName] = headValue;
    return true;
}

void HttpRequest::SetQuery(const char *begin, const char *end)
{
    assert(*begin == '?');
    query_.assign(begin + 1, end);
}

std::string  const HttpRequest::nullString = "";

}
}
