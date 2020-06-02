#pragma once
#include <cstddef>
#include<vector>
#include<string>
class Buffer
{
public:
    Buffer(size_t bufferBeginLength = kDefaultSize)
        :readIndex_(kPrependable),
         writeIndex_(kPrependable)
    {
        buffer_.resize(bufferBeginLength);
    }
    ~Buffer() {}

    void Append(std::string &str);
    void Append(const char* msg, size_t length);
    void Append(const void* msg, size_t length);
    void Append(int number);
    void Append(long number);
    void Append(unsigned int number);

    void Retrieve(size_t length);
    std::string RetrieveAsString(size_t length);
    std::string RetrieveAllAsString();
    void RetrieveAll();

    //For HttpServer
    const char* FindCRLF();
    void RetrieveUntilCRLF();
    

    size_t DataNumber() { return writeIndex_ - readIndex_; }
    void shrink();

    size_t ReadFd(int sockfd);    ///<读套接字数据到应用层缓冲

    const char* Peek()  {   return &*(buffer_.begin() + readIndex_); }
    
    size_t BackEndCapacity()  { return buffer_.size() - writeIndex_; }  ///<求出尾后空余空间，这决定了数据是否要内部迁移
private:
    char* Begin()       {   return &*buffer_.begin();}
    char* End()         {   return &*(buffer_.end());}
    std::vector<char>::iterator WritablePlace()  { return buffer_.begin() + writeIndex_; }
    std::vector<char>::iterator ReadablePlace()  { return buffer_.begin() + readIndex_;  }
    void MakeSpace(size_t length);
    size_t Capacity()   {   return buffer_.size() - (writeIndex_ - readIndex_) - kPrependable;} ///<求出容器可放的数据空位
    void MoveDataToHead();      ///<当尾后数据空位不够而容器本身空余size够时则发生内部数据迁移（muduo 缓冲区）
    void AddData(const char *msg, size_t length);        ///<Append的底层调用
    void RemoveData(size_t length);                     ///<Retrieve的底层调用

private:
    std::vector<char> buffer_;
    size_t readIndex_;
    size_t writeIndex_;
    const static int kPrependable = 8;
    const static int kDefaultSize = 1024;

    //For Http
    const static char kCRLF[2] ;

};

