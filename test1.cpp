#include"test1.hpp"

Test2::Test2()
{
}

Test2::~Test2()
{
}

void Test2::print_test_head(Test &test)
{
    std::map<std::string, std::string>::iterator it;

    for(it = test.head.begin(); it != test.head.end(); it++)
    {
        std::cout << it->first << " : " << it->second << std::endl;
    }
}
