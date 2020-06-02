#include <iostream>
#include <netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
int main(int argc, char**argv)
{
    if(argc != 2)
        return -1;
    int port = atoi(argv[1]);
    std::cout<<"port is "<<port<<std::endl;

    int socket = ::socket(AF_INET, SOCK_STREAM ,0);

    
    struct sockaddr_in server_addr;
    memset(&server_addr , 0, sizeof(sockaddr_in));
    inet_pton(AF_INET,"127.0.0.1" , &server_addr.sin_addr.s_addr );
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;


    int isok = ::connect(socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(isok == 0)
        std::cout<<"connect ok!"<<std::endl;
    else
        std::cout<<"fail !"<<std::endl;
    std::cout<<errno<<std::endl;


    std::cout << "Hello world" << std::endl;
    return 0;
}

