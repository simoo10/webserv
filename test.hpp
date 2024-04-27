#ifndef TEST_HPP
#define TEST_HPP

#include<iostream>
#include<string>
#include<map>
#include"test1.hpp"
class Test
{
public:
    Test();
    ~Test();
    void fill_head();
    std::map<std::string, std::string> head;
};

#endif

