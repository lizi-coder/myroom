#include <cstdio>
#include <cstring>
#include<string>
#include<assert.h>
#include"AppendFile.h"


const char* base::AppendFile::hostname_ = "lizi";
const char* base::AppendFile::backendName_ = "log";

void base::AppendFile::append(const char *msg, int length)
{
    if(msg == NULL)
        write("(null)", 6);
    else
        write(msg, length);
}
void base::AppendFile::flush()
{
    ::fflush(fileStream_);

}
void base::AppendFile::remove()
{

}

void base::AppendFile::write(const char *msg, int length)
{
    assert(msg != NULL);
    int n = ::fwrite_unlocked(msg, 1, length, fileStream_); ///<知识点：无锁写，那就意味着fwrite本身是有锁的
    assert(n >= 0);
    int remain = length - n;
    while(remain > 0)
    {
        n = ::fwrite_unlocked(msg + n, 1, length - remain, fileStream_);
        remain -= n;
        if(n < 0)
            ferror_unlocked(stdout);
    }
    writtenBytes += length;
}

size_t base::AppendFile::GetWrittenBytes()
{
    return writtenBytes;
}

