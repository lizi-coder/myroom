#pragma once

/**
 * 日志入口类：负责控制日志级别
 * 工作流程：
 */


#include <cstddef>
#include<pthread.h>
#include<assert.h>
#include<string.h>
#include<functional>
#include"TimeStamp.h"
#include"NonCopyable.h"
#include"LogStream.h"
#include"CurrentThread.h"


#include<stdio.h>
#define LOG_FATAL   base::Logging(__FILE__,__LINE__, Logging::FATAL, errno).GetStream() 
#define LOG_ERROR   if(Logging::GetLogRank() <= Logging::ERROR) \
        base::Logging(__FILE__,__LINE__, Logging::ERROR, errno).GetStream() 
#define LOG_DEBUG if(Logging::GetLogRank() <= Logging::DEBUG) \
        base::Logging(__FILE__,__LINE__, Logging::DEBUG).GetStream() 
#define LOG_TRACE if(Logging::GetLogRank() <= Logging::TRACE) \
        base::Logging(__FILE__,__LINE__, Logging::TRACE).GetStream() 
#define LOG_WARN  if(Logging::GetLogRank() <= Logging::WARN) \
        base::Logging(__FILE__,__LINE__, Logging::WARN).GetStream()  
#define LOG_INFO   if(Logging::GetLogRank() <= Logging::INFO) \
        base::Logging(__FILE__,__LINE__, Logging::INFO).GetStream()  
namespace base
{

class Logging : base::NonCopyable
{
public:
    
    class FileName;
    enum LOG_RANK
    {
        FATAL = 1,
        ERROR,
        DEBUG,
        TRACE,
        WARN,
        INFO,
        OFF
    };

    typedef std::function<void(const char*, size_t)> StreamOutputFunc;
    typedef std::function<void()> StreamFlushFunc;
    Logging(FileName name,int line , LOG_RANK rank);
    Logging(FileName name,int line , LOG_RANK rank, int savederrno);
    ~Logging();

    static LOG_RANK GetLogRank()
    {
        return CurrentLogRank_;
    }
    static const char* GetLogRankAsChar(LOG_RANK);
    static bool SetLogRank(std::string rank); ///<这里用字符串匹配，有什么更好的办法去匹配用户传入的日志级别？
    static void SetOutPut(StreamOutputFunc func); ///<用户自定义设置缓冲输出方法
    static void SetFlush(StreamFlushFunc func); ///<用户自定义冲刷方
    LogStream& GetStream()
    {
        return impl_.GetStream();
    }
    
public:

    /**
     * 作用：求出文件名及长度存储到FileName类对象中
     * 问题：用char* 还是 const char*？
     * 问题：__FILE__输出的含路径还是不含路径？
     */
    class FileName
    {
    public:
        FileName(const char *data)
        {
            assert(data != NULL);
            char *namePlace = const_cast<char*>(strrchr(data, '/'));
            //assert(namePlace != NULL);
            if(namePlace == NULL)   ///<完整路径没有找着\? 正常情况下不会发生这种情况
            {
                data_ = const_cast<char*>(data);
                fileLength_ = strlen(data);
            }
            else
            {
                data_ = namePlace + 1;
                fileLength_ = strlen(data_);
            }
        }
        explicit FileName(std::string &data)
        {
            assert(data.size() != 0);
            std::string::size_type namePlace = data.rfind('\\');
            assert(namePlace != std::string::npos);
            data_ = const_cast<char*>(data.c_str()) + namePlace + 1;
            fileLength_ = data.size() - 1 - namePlace;

        }
        FileName(FileName && that)
        {
            assert(that.data_ != NULL);
            fileLength_ = that.fileLength_;
            data_ = that.data_;
            that.data_ = NULL;
            that.fileLength_ = 0;
        }
        const char* GetFileName()
        {
            return data_;
        }
        size_t GetFileNameLength()
        {
            return fileLength_;
        }

    private:
        char *data_;
        size_t   fileLength_;
    };  // END OF class FileName

private:
    void finish()
    {
        ///<注意：实际上的日志是不允许有FileName、CodeLine等字样（占空间）
        ///<这边是纯粹为了自己看方便而作的
    //    GetStream()<<" - "<<"FileName:"<<name_.GetFileName()<<" "<<"CodeLine:"<<codeLine_<<"\n";
        LogStream& stream = GetStream();
        if(rank_ <= ERROR)
        {
            char errorInfoBuf[64] = { 0 }; 
            stream<<" "<<"errno:"<<savederrno_<<","<<strerror_r(savederrno_, errorInfoBuf, sizeof errorInfoBuf);
        }
        stream<<"\n";
    }
    static void defaultOutput(const char *msg, size_t length)
    {
        assert(msg != NULL && length > 0);
        ::fwrite(msg,1, length, stdout);
    }
    static void defaultFlush()
    {
        ::fflush(stdout);
    }
private:
class Impl  ///<为什么要这么设计？
{
public:
    Impl(FileName name,unsigned int line, LOG_RANK rank, int savederrno);
    Impl(FileName name, unsigned int line, LOG_RANK rank);
    LogStream& GetStream() 
    {
        return stream_;
    }
    bool GetAbortFlag()
    {
        return toAbort;
    }
    /*
    const char* GetFileName()
    {
        return name_.GetFileName();
    }
    */
private:
    FileName name_;
    unsigned int codeLine_;
    pid_t tid_;
    LOG_RANK rank_;
    int savederrno_;
    LogStream stream_;
    TimeStamp time_;
    bool toAbort;       ///< for LOG_FATAL ONLY!
}; //END OF class Impl

private:
    int savederrno_;
    LOG_RANK rank_;
    Impl impl_;
    static LOG_RANK CurrentLogRank_;
    static StreamOutputFunc o_output_;
    static StreamFlushFunc  o_flush_; ///<static限制了程序只能有一种行为
}; //END OF class Logging

}   //END OF namespace base
