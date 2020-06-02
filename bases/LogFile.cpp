#include"LogFile.h"
#include "AppendFile.h"
#include"MutexLock.h"
#include"MutexLockGuard.h"
#include "TimeStamp.h"
#include<assert.h>
#include<string.h>
namespace base
{

LogFile::LogFile(const char* basename,size_t rollSize ,
                 bool isThreadSafe, int maxCount)
    :lastFlushTime_(0),   
     lastRollTime_(0),
     count_(0),
     maxCount_(maxCount),
     rollSize_(rollSize),
     file_(new AppendFile(basename)),
     basename_(basename),
     isThreadSafe_(isThreadSafe),
     mutex_(isThreadSafe ? new MutexLock() : NULL)
{
    assert(basename!= NULL);
    assert(strchr(basename, '/') == NULL);
}        

void LogFile::Append(const char *msg, size_t length)
{
    if(isThreadSafe_)
    {
        MutexLockGuard(*mutex_);
        AppendUnLock(msg, length);
    }
    else
        AppendUnLock(msg, length);
}


/**
 * 写数据策略：1.先写数据，写完后根据WrittenBytes判断
 *             2 
 *              
 */
void LogFile::AppendUnLock(const char* msg, size_t length)
{
    file_->append(msg, length);
    size_t writtenBytes = file_->GetWrittenBytes();
    if(writtenBytes > rollSize_)
    {
        RollFile();
    }
    else
    {
        count_++;
        if(count_ >= maxCount_)
        {
            count_ = 0;
            time_t now = time(NULL) % kDefaultRollTimeSecond;
            if(now > lastRollTime_)
            {
                lastRollTime_ = now;
                RollFile();
            }
            else
            {
                lastFlushTime_ = now;
                Flush();
            }
        }
    }
}

void LogFile::RollFile()
{
    Flush();
    time_t now = time(NULL) % kDefaultRollTimeSecond;
    lastRollTime_ = now;
    lastFlushTime_ = now;
    file_.reset(new AppendFile(basename_));
}

void LogFile::Flush()
{
    file_->flush();
}
}
