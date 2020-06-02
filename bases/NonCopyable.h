#ifndef _BASE_NONCOPYABLE_
#define _BASE_NONCOPYABLE_
namespace base
{

class NonCopyable
{
public:
    NonCopyable() {}
    ~NonCopyable() {}
private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable& that);
};
}

#endif
