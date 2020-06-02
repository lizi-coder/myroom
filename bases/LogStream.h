#pragma once
#include<string>
#include<string.h>
#include<assert.h>


namespace base
{
const static unsigned int kbufferSize = 4000; ///< 缓冲区默认大小，如果不够从这改

template<unsigned int N>
class FixedBuffer
{
public:
    FixedBuffer()
        :cur_(buf_),
         data_(buf_)
    {}
    FixedBuffer(const FixedBuffer & that)
    {
        cur_ = that.cur_;
        data_ = that.data_;
        memcpy(buf_, that.buf_, sizeof buf_);
    }
    ~FixedBuffer() {}

    //// 这儿提供些缓冲区常见方法
    
    bool Append(const char *msg, int len)
    {
        //printf("test append msg %s\n",msg);
     //   assert(msg != NULL && len > 0);
    //    assert(len <= &buf_[N] -cur_);
        if(len > &buf_[N] - cur_ )
            len = &buf_[N] - cur_;
        memcpy(cur_, msg, len);
        cur_ += len;
        return true;
    }
    bool Append(char msg)
    {
        if(cur_ < buf_ + N)
        {
            *cur_++ = msg;
            return true;
        }
        return false;
    }
    void remove(int removeNum)
    {
        assert(removeNum >= 0);
        if(removeNum >= avail())
            cur_ = buf_;
        else
            cur_ -= removeNum;
    }
    void removeAll()
    {
        cur_ = buf_;
    }
    const char* GetDataAsConst() const
    {
        *cur_ = 0;
        return data_;
    }
    char* GetData()
    {
        *cur_ = 0;
        return data_;
    }
    const std::string GetDataAsString() const 
    {
        return std::string(data_, cur_);
    }
    size_t avail()
    {
        return static_cast<size_t>(cur_ - data_);
    }

private:
    char buf_[N];
    char *cur_;
    char *data_;

};

/**
 * 功能：日志缓冲区类
 * 难点：如何实现变长的char[]？
 */
class LogStream
{
public:
    typedef base::FixedBuffer<kbufferSize> Buffer;

    LogStream() {}
    ~LogStream() {}
    LogStream& operator<<(bool data)
    {
        buffer_.Append(data == true? '1': '0');
        return *this;
    }
    LogStream& operator<<(const char* msg)
    {
        assert(msg != NULL);
        bool isOK = buffer_.Append(msg, strlen(msg));
        assert(isOK == true);
        return *this;
    }
    LogStream& operator<<(std::string msg)
    {
        const char *data = msg.data();
        assert(data != NULL);
        bool isOK = buffer_.Append(data, msg.size());
        assert(isOK == true);
        return *this;
    }
    LogStream& operator<<(int number)  ///<数字仅支持int，其他的道理都一样，省略了
    {
        char tempBuf[20] = { 0 };
        NumberToChar(tempBuf, sizeof tempBuf, number);
        buffer_.Append(tempBuf, strlen(tempBuf));
        return *this;
    }
    LogStream& operator<<(unsigned int number)  ///<数字仅支持int，其他的道理都一样，省略了
    {
        char tempBuf[20] = { 0 };
        NumberToChar(tempBuf, sizeof tempBuf, number);
        buffer_.Append(tempBuf, strlen(tempBuf));
        return *this;
    }
    LogStream& operator<<(unsigned long number)
    {
        char tempBuf[40] = { 0 };
        NumberToChar(tempBuf, sizeof tempBuf, number);
        buffer_.Append(tempBuf, strlen(tempBuf));
        return *this;
    }

    LogStream& operator<<(double number)
    {
        std::string NumberStr = std::to_string(number);
        buffer_.Append(NumberStr.data(), NumberStr.size());

        return *this;
    }

    Buffer& getBuffer()
    {
        return buffer_;
    }
    size_t avail()
    {
        return buffer_.avail();
    }

private:
    void NumberToChar(char *buf, int bufLength, int number) ///<简化版，只提供了int类型的转化
    {
        
        int i = 0;  ///<用于数组翻转
        assert(buf != NULL && bufLength >= 11);     ///< 11是指int最大长度（10）加上正负号
        int temp = number;
        char targetNum[11]  = "0123456789";
        char toReverseBuf[11] = { 0 };
        int bufCur = 0;
        if(temp < 0)
        {
            buf[bufCur++] = '-';
            temp = -temp;
            i++;
        }
        do
        {
            int last = temp % 10;
            temp = temp / 10;
            toReverseBuf[bufCur++] = targetNum[last];
        }while(temp);
        assert(bufCur > 0);
        while(bufCur > 0)
        {
            buf[i++] = toReverseBuf[--bufCur];
        }
    }

private:
    Buffer buffer_;

};

}
