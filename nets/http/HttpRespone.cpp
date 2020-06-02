#include<string>
#include<algorithm>
#include"HttpRespone.h"
#include"../../bases/Logging.h"
using namespace base;
using namespace std;
namespace net
{
namespace http
{


std::string HttpRespone::AppendToString()
{
    string res("");

    //构造响应行
    res = res + GetVersionAsString() + " " + to_string(type_)+ " " + GetResponeTypeAsString() + crlf;
    //构造头部行
//    std::for_each(headers_.begin(), headers_.end(), [&](pair<HeadName,HeadValue> &headpair) { res = res + headpair.first + ": " + headpair.second + crlf; });

    for(auto &headpair : headers_)
    {
        res = res + headpair.first + ": " + headpair.second + crlf;
    }

    //构造头部行和body之间的crlf
    res += crlf;

    LOG_DEBUG<<"Response Header is "<<"\n"<<res;

    //构造body
    res = res + body_ ;

    

    //LOG_DEBUG<<"respone is "<<res;
    
    return res;
}


const char* HttpRespone::crlf = "\r\n";


















}
}
