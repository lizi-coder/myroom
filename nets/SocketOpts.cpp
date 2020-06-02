#include <asm-generic/socket.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <unistd.h>
#include"../bases/Logging.h"
#include"Socket.h"
#include"InetAddr.h"
using namespace base;
namespace net
{

namespace socket
{

int CreateSocket()
{
    int socket = ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    if(socket < 0)
    {
        LOG_FATAL <<"CreateSocket fail!";
    }
    return socket;
}



}


}


