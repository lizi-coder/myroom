#include"Logging.h"
#include "LogStream.h"
#include "TimeStamp.h"
#include"CurrentThread.h"
#include <functional>

namespace base
{
    const static Logging::LOG_RANK kDefaultLogRank = Logging::DEBUG;     ///<默认日志类型实际上是INFO，为了写程序方便就用了DEBUG
    Logging::LOG_RANK Logging::CurrentLogRank_ = kDefaultLogRank;
    Logging::StreamOutputFunc Logging::o_output_ = Logging::defaultOutput;
    Logging::StreamFlushFunc Logging::o_flush_ = Logging::defaultFlush;

    Logging::Logging(FileName name,int line , LOG_RANK rank)
        :savederrno_(0),
         rank_(rank),
         impl_(std::move(name), line, rank)
         //o_output(std::bind(&Logging::defaultOutput,GetStream())),  ///<严重BUG：此处它会把stream拷贝一份到bind里面，写的时候会去写里面的拷贝！
         //name_(std::move(name)),
         //codeLine_(line),
    {
    }
    Logging::Logging(FileName name,int line , LOG_RANK rank, int savederrno)
        :savederrno_(savederrno),
         rank_(rank),
         impl_(std::move(name), line, rank, savederrno)
         //name_(std::move(name)),
         //codeLine_(line),
    {
    }
    Logging:: ~Logging()
    {
        //printf("~logging\n");
        finish();
        LogStream::Buffer &buffer = GetStream().getBuffer();
        o_output_(buffer.GetDataAsConst(), buffer.avail());
        o_flush_();
        if(impl_.GetAbortFlag())
           abort();
    }
    const char* Logging::GetLogRankAsChar(LOG_RANK rank = GetLogRank())
    {   
        switch(rank)
        {
        case INFO:
            return "INFO ";
        case FATAL:
            return "FATAL";
        case ERROR:
            return "ERROR";
        case DEBUG:
            return "DEBUG";
        case TRACE:
            return "TRACE";
        case WARN:
            return "WARN ";
        case OFF:
            return "OFF  ";
        default:
            return "UNKWN";
        }
    }

    bool Logging::SetLogRank(std::string rank)
    {
        bool isOk = true;
        if(rank == "INFO")
            CurrentLogRank_ = INFO;
        else if(rank == "WARN")
            CurrentLogRank_ = WARN;
        else if(rank == "TRACE")
            CurrentLogRank_ = TRACE;
        else if(rank == "DEBUG")
            CurrentLogRank_ = DEBUG;
        else if(rank == "OFF")
            CurrentLogRank_ = OFF;
        else 
            isOk = false;
        assert(isOk == true);
        return isOk;
    }
    void Logging::SetOutPut(StreamOutputFunc func)
    {
        o_output_ = func;
    }
    void Logging::SetFlush(StreamFlushFunc func)
    {
        o_flush_ = func;
    }

    Logging::Impl::Impl(FileName name,unsigned int line, LOG_RANK rank, int savederrno)
       :name_(std::move(name)),
        codeLine_(line),
        tid_(CurrentThread::Tid()),
        rank_(rank),
        savederrno_(savederrno),
        stream_(),
        time_(TimeStamp::now()),
        toAbort(rank == Logging::FATAL? true: false)
    {
        stream_<<std::move(time_.GetFormatTime())<<\
            " ThreadTid:"<<tid_<<" "<<Logging::GetLogRankAsChar(rank_)<<\
       " - "<<name_.GetFileName()<<" "<<"CodeLine:"<<codeLine_<<" output:";
            //" "<<"errno:"<<savederrno<<" ";
    }
    Logging::Impl::Impl(FileName name, unsigned int line, LOG_RANK rank)
       :name_(std::move(name)),
        codeLine_(line),
        tid_(CurrentThread::Tid()),
        rank_(rank),
        stream_(),
        time_(TimeStamp::now()),
        toAbort(false)
    {
        stream_<<std::move(time_.GetFormatTime())<<\
            " ThreadTid:"<<tid_<<" "<<Logging::GetLogRankAsChar(rank_)<<\
       " - "<<name_.GetFileName()<<" "<<"CodeLine:"<<codeLine_<<" output:";
    }

}
