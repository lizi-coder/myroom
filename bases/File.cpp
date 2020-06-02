
#include"File.h"
#include <cstdio>
#include"Logging.h"


namespace base
{

File::File()
    :open_(false),
     fp_(NULL)
    {}

File::File(const char *path)
{
    File();
    fp_ = fopen(path, "ba+");
    if(fp_ == NULL)
    {
        LOG_ERROR<<"FILE:"<<path<<" Open Fail!";
    }
    else
    {
        LOG_DEBUG<<"FILE:"<<path<<" Open Scuess!";
        path_ = path;
        open_ = true;
    }
}

File::File(const std::string path)
{
    File(path.c_str());
}

File::~File()
{
    if(open_)
        fclose(fp_);
}


size_t File::ReadFile(size_t N, std::string &data)
{
    if(N > 1024 * 1024 * 4)
    {
        LOG_WARN<<"ReadFile n too large!!";
        N = 1024 * 1024 * 4;
    }
    if(!open_)
    {
        LOG_ERROR<<"File Not Open!";
        return -1;
    }

    data.reserve(N);
    size_t nread = fread_unlocked(&*data.begin(), N, 1, fp_);
    if(nread < 0)
    {
        LOG_ERROR<<"Read File Error!";
    }
    return nread;

}
size_t File::AppendFile(std::string &data)
{
    if(!open_)
    {
        LOG_ERROR<<"File Not Open!";
        return -1;
    }
    size_t nwrite = fwrite_unlocked(data.c_str(), 1, data.length(), fp_ );
    if(nwrite< 0)
    {
        LOG_ERROR<<"Write File Error!";
    }
    return nwrite;
}































}
