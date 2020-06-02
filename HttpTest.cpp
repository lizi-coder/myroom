#include <cstdio>
#include <iostream>
#include<ulimit.h>
#include "nets/EventLoop.h"
#include "nets/http/HttpRequest.h"
#include "nets/http/HttpRespone.h"
#include "nets/http/HttpServer.h"
#include "bases/Logging.h"
#include "bases/AppendFile.h"

using namespace std;
using namespace net;
using namespace net::http;
using namespace base;

char buf[99999999] = { 0 }; 
FILE *p = ::fopen("./7-1.jpg", "rb");
int n = ::fread(buf, 1, sizeof(buf), p);
void Handler(const HttpRequest &req, HttpRespone *res)
{

    LOG_INFO<<"read picture "<<n<<" bytes";

    LOG_INFO<<"New Connection!---------------------------------------------";
    LOG_INFO<<"RECEIVE TIME:"<<req.GetHeader("Date");

    res->SetVersion('1');
    res->SetResponeType(net::http::HttpRespone::kOK);
    res->SetHeader("Content-Type", "image/jpg");
    res->SetHeader("Content-Length", to_string(n));

    /*
    std::string body = "\
        <html>      \
        <body>\
        <h1>HELLO!WORLD!!!!!!!!!</h1>\
        <p>MY FIRST PAGE!</p>\
        </body>\
        </html>";
*/
    res->SetBody(string(buf, n));


}


int main()
{

    EventLoop loop;
    //Logging::SetLogRank("OFF");
    HttpServer server(&loop, "192.168.83.145", 8000);
    server.SetThreadNum(10);

    server.SetHandlerCallBack(Handler);
    
    server.Start();

    loop.Loop();
    



    std::cout << "Hello world" << std::endl;
    return 0;
}

