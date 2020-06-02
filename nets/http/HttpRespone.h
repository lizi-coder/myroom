#pragma once

#include <map>
namespace net
{
namespace http
{


class HttpRespone
{
public:
    enum Version { kInValid, kHttp10, kHttp11 };
    enum ResponeType { kUnknowType = 403, kOK = 201, kAccept = 200, kBadRequest = 400, kNotFound = 404 };
    typedef std::string HeadName;
    typedef std::string HeadValue;
    
    HttpRespone() {}
    ~HttpRespone() {}

    bool SetVersion(const char version)
    {
        if(version == '1')
            version_ = kHttp11;
        else if(version == '0')
            version_ = kHttp10;
        else
            return false;

        return true;
    }
    void SetResponeType(ResponeType type)   {type_ = type;}
    void SetBody(const std::string &body)   {body_ = body;}
    void AppendData(const std::string &body){body_ += body;}
    void SetHeader(const std::string& headname, const std::string& headvalue)   { headers_[headname] = headvalue; }

    ResponeType GetResponeType()  const  {return type_;}
    std::string GetResponeTypeAsString()  const
    {
        switch (type_)
        {
        case kAccept:
        case kOK:
            return "Accept";
        case kNotFound:
            return "Not Found";
        case kBadRequest:
        case kUnknowType:
        default:
            return "Bad Request";
        }
    }
    std::string GetHeader(const std::string& headname)   const
    {
        auto it = headers_.find(headname);
        if(it != headers_.end())
            return it->second;
        else
            return "";
    }
    std::string GetVersionAsString() const
    {
        if(version_ == kHttp10)
            return "HTTP/1.0";
        else if(version_ == kHttp11)
            return "HTTP/1.1";
        else
            return "";
    }
    std::string AppendToString();

private:
    Version version_;
    ResponeType type_;
    std::string explain_;
    std::map<HeadName, HeadValue> headers_;
    std::string body_;

    static const char* crlf;

};


}

}

