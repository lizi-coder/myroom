#include"Buffer.h"
#include"../bases/Logging.h"
#include <bits/types/struct_iovec.h>
#include <cstddef>
#include <iterator>
#include <string>
#include<sys/uio.h>
#include<algorithm>
#include <vector>
using namespace base;
void Buffer::Append(std::string &str)
{
    size_t targetSize = str.size();
    if(targetSize == 0)
    {
        LOG_WARN<<"Append MSG is NULL!!";
        return;
    }
}
void Buffer::Append(const char* msg, size_t length)
{
    if(msg == NULL || length == 0)
    {
        LOG_WARN<<"Append MSG is NULL!!";
        return;
    }
    AddData(msg, length);
}
void Buffer::Append(const void* msg, size_t length)
{
    if(msg == NULL || length == 0)
    {
        LOG_WARN<<"Append MSG is NULL!!";
        return;
    }
    AddData(static_cast<const char*>(msg), length);
}
void Buffer::Append(int number)
{
    AddData(reinterpret_cast<const char*>(&number), sizeof(number));
}
void Buffer::Append(long number)
{
    AddData(reinterpret_cast<const char*>(&number), sizeof(number));
}
void Buffer::Append(unsigned int number)
{
    AddData(reinterpret_cast<const char*>(&number), sizeof(number));
}

void Buffer::Retrieve(size_t length)
{
    if(length < DataNumber())
        readIndex_ += length;
    else
        RetrieveAll();
}
std::string Buffer::RetrieveAsString(size_t length)
{
    if(length < DataNumber())
    {
        readIndex_ += length;
        return std::string(Peek(), Peek() + length);
    }
    else
    {
        return RetrieveAllAsString();
    }
    return Peek();

}
void Buffer::RetrieveAll()
{
    readIndex_  = kPrependable;
    writeIndex_ = kPrependable;
}
std::string Buffer::RetrieveAllAsString()
{
    LOG_DEBUG<<"now Retrieve data as string...dataNum:"<<DataNumber();
    std::string result(Peek(), Peek() + DataNumber());
    readIndex_  = kPrependable;
    writeIndex_ = kPrependable;
    return result;
}



const char* Buffer::FindCRLF()
{
    std::vector<char>::iterator crlf = std::search(ReadablePlace(), WritablePlace(), kCRLF, kCRLF + 2);
    return crlf == ReadablePlace() ? NULL : &*crlf;
}

void Buffer:: RetrieveUntilCRLF()
{
    std::vector<char>::iterator crlf = std::search(ReadablePlace(), WritablePlace(), kCRLF, kCRLF + 2);
    LOG_DEBUG<<"before readindex is "<<readIndex_;
    readIndex_ += crlf - ReadablePlace() + 2;

    LOG_DEBUG<<"Now ReadablePlace is "<<readIndex_;
}


void Buffer::shrink()
{

}

size_t Buffer::ReadFd(int sockfd) ///<利用分散读
{
    /*
    char exterBuf [65532];
    int n = ::read(sockfd, exterBuf, sizeof(exterBuf));
    LOG_TRACE<<"ReadFd read "<<n<<" bytes";
    if(n > 0)
        Append(exterBuf, n);
    return n;
    */
    char extrabuf [65532];
    struct iovec buf[2];
    buf[0].iov_base = &*WritablePlace();
    buf[0].iov_len  = BackEndCapacity();        ///<注意我们是向尾后添加数据，那么应该是尾巴的容量
    buf[1].iov_base = extrabuf;
    buf[1].iov_len  = 65532;

    volatile size_t n = readv(sockfd, buf, 2);
    LOG_TRACE<<"ReadFd read "<<n<<" bytes";
    if(n <= 0)
        return n;
    else
    {
        if(n > BackEndCapacity())      ///<如果说stack上有读入数据
        {
           LOG_DEBUG<<"Now Append data, length"<<n - buf[0].iov_len<<" BackEndCapacity length is "<<BackEndCapacity()<<" n = "<<n;
           writeIndex_ += BackEndCapacity();
           //Append(extrabuf, n - BackEndCapacity());        ///<错误写法！当数据已输入时，BackEndCapacity在此处就已经变为0了
           if(n - buf[0].iov_len > 0)
               Append(extrabuf, n - buf[0].iov_len);
        }
        else 
            writeIndex_ += n;
        return n;
    }
}

void Buffer::MakeSpace(size_t length)
{
    size_t targetLength = buffer_.size() + length;

    if(targetLength <= buffer_.size())
    {
        LOG_DEBUG<<"MakeSpace Error! targetLength = "<<targetLength<<",length:"<<length<<" ,buffer_size is "<<buffer_.size();
        return;
    }
    //assert(targetLength > buffer_.size());
    LOG_DEBUG<<"now MakeSpace to "<<targetLength;
    buffer_.resize(targetLength);
}

void Buffer::MoveDataToHead()
{
    size_t totalDataNumber = DataNumber();
    std::copy(ReadablePlace(),WritablePlace(), buffer_.begin()+kPrependable );
    readIndex_  = kPrependable;
    writeIndex_ = readIndex_ + totalDataNumber;
}

void Buffer::AddData(const char *msg, size_t length)        ///<Append的底层调用
{
    if(length <= 0 || msg == NULL)
        return;
    if(BackEndCapacity() < length)
    {
        if(Capacity() >= length)
            MoveDataToHead();
        else
            MakeSpace(length);
    }
    LOG_DEBUG<<"Copy data to WritablePlace..., now WritablePlace is "<<writeIndex_;
    std::copy(msg, msg + length, WritablePlace());
    writeIndex_ += length;
}
void Buffer::RemoveData(size_t length)                     ///<Retrieve的底层调用
{
    if(length > DataNumber())
    {
        LOG_WARN<<"Peek Too Much Data!!";
        length = DataNumber();
    }
    readIndex_ += length;
    if(readIndex_ == writeIndex_)       ///<在之后相遇意味着数据读满了
    {
        readIndex_  = kPrependable;
        writeIndex_ = kPrependable;
    }

}

const char Buffer::kCRLF[2]{'\r','\n'};


