#pragma once

/**
 * 
 * 功能：日志文件类
 * 日志文件格式例：logfile_test.20200506-165030.hostname.3605.log
 *                  basename       time           host   3605:process-id
 */


#include "TimeStamp.h"
#include <cstdio>
#include<string.h>
#include<string>
namespace base
{

class AppendFile
{
public:
    explicit AppendFile(const char* basename)
        :basename_(basename),
         filename_(basename_),
         writtenBytes(0)
    {
        filename_ = filename_ +"." + TimeStamp::now().GetFormatTime() + ".";
        filename_ = filename_ + hostname_+ "." + backendName_;
        fileStream_ = ::fopen(filename_.c_str(), "a+");
        ::setbuffer(fileStream_, buffer_, sizeof buffer_);
    }
    ~AppendFile() {
        ::fclose(fileStream_);
    }
public:
    void append(const char *msg, int length);
    void flush();
    void remove();
    void write(const char *msg, int length);
    size_t GetWrittenBytes();

private:
    const static size_t kDefaultBufferSize = 64 * 1024; 

    const char* basename_;      ///<文件根名
    const static char* hostname_;      ///<hostname
    const static char* backendName_ ;
    std::string filename_;      ///<总文件名

    char buffer_[kDefaultBufferSize];   ///<应用层缓冲，放弃使用系统提供的缓冲
    size_t writtenBytes;

    FILE* fileStream_;
};

}
