#include "flat_list.h"
#include <iostream>

struct test_object
{
    test_object(int i) : value(i)
    {
    }

    int value;
};

int main()
{
    util::flat_list<test_object> l(15);

    for(int i=1; i<=16; ++i)
    {
        auto ret = l.emplace_back(i);
        if(ret == l.end())
        {
            std::cerr << "fail to emplace_back: " << i << std::endl;
        }
    }

    auto beg = l.begin();
    for(int i = 1; i <= 32; ++i)
    {
        std::cout << (*beg).value << std::endl;
        ++beg;
    }

    return 0;
}

