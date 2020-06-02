#include <iostream>
#include <typeinfo>

struct hello
{
    int a;
    char b;
};
int main()
{
    hello h1;
    //int a = 5;

    const std::type_info& info1 = typeid("hello");

    std::cout<<info1.name()<<" "<<info1.hash_code()<<" "<<info1.before(info1);

    return 0;
}

