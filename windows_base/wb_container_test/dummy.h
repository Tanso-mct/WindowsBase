#pragma once

#include <iostream>

class Dummy
{
private:
    int value_;

public:
    Dummy(int value) : value_(value) 
    {
        std::cout << "Dummy created with value: " << value_ << std::endl;
    }

    ~Dummy()
    {
        std::cout << "Dummy destroyed with value: " << value_ << std::endl;
    }

    int GetValue() const { return value_; }
};

enum class DummyID : size_t
{
    ID0 = 0,
    ID1,
    Size,
};