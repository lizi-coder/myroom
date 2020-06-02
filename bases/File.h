#pragma once
#include <cstddef>
#include<string>
#include <bits/types/FILE.h>

namespace base
{




//读写文件类
class File
{
public:
    File();
    File(const char *path);
    File(const std::string path);
    ~File();

    //template<size_t N>
    size_t ReadFile(size_t, std::string &data);
    size_t ReadFile(std::string &data);
    size_t AppendFile(std::string &data);

private:
    bool open_;
    FILE *fp_;
    std::string path_;

};

}
