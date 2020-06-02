#pragma once

#include <bits/types/time_t.h>
#include<memory>
#include"AppendFile.h"
#include"NonCopyable.h"
#include"MutexLock.h"

/**
 * 功能：日志滚动类
 * 作用：每隔一段时间(krollTime)进行日志滚动或者每隔一定秒数进行文件冲刷fflush
 * 滚动条件：1.日志条数超过kMaxCount_并超过规定时间
 *           2.日志文件大小超过用户指定的rollSize   
 * 冲刷条件：1.日志滚动时
 *           2.日志条数超过kMaxCount，但未超过规定时间   
 * 注意事项：线程安全性由本类（LogFile）保证，AppendFile类不保证线程安全！
 */

namespace base
{


class LogFile : NonCopyable
{
public:
    LogFile(const char* basename,size_t rollSize = kDefaultRollFileSize,
            bool isThreadSafe = true,int maxCount = kDefaultCountMaxNumber);
    ~LogFile() = default;

    void Append(const char*, size_t);
    void Flush();

private:
    void AppendUnLock(const char*, size_t);
    void RollFile();
    

private:
    const static time_t kDefaultRollTimeSecond  = 60 * 10; ///<10分钟滚动一次日志（测试用）
    const static time_t kDefaultFlushTimeSecond = 3;
    const static size_t kDefaultRollFileSize    = 60 * 1024;
    const static int    kDefaultCountMaxNumber     = 1024;
    
    time_t lastFlushTime_;    ///<这个如何初始化？是初始化为0还是当前时间取整？
    time_t lastRollTime_;

    int count_;     ///<日志条数计数
    int maxCount_;
    size_t rollSize_; ///<文件达到多大就滚动一次，由这个指定

    std::unique_ptr<AppendFile> file_;
    const char* basename_;

    bool isThreadSafe_;
    std::unique_ptr<MutexLock>  mutex_;

};  //END OF class LogFile

}
