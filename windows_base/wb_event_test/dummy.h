#pragma once

#include "wb_interface/event.h"

#include <iostream>

class DummyEvent : public wb::IEvent
{
private:
    const int id_ = 0;

public:
    DummyEvent(int id = 0) : id_(id)
    {
        std::cout << "DummyEventのコンストラクタが呼ばれた。IDは" << id_ << std::endl;
    }

    ~DummyEvent() override
    {
        std::cout << "DummyEventのデストラクタが呼ばれた。IDは" << id_ << std::endl;
    }

    bool IsDestroying() override
    {
        return false;
    }

    void DummyFunction(int value, float rate)
    {
        std::cout << "DummyFunctionが呼び出された。IDは" << id_ << " ";
        std::cout << "値: " << value << ", レート: " << rate << std::endl;
    }

    void OtherFunction(int value, float rate)
    {
        std::cout << "OtherFunctionが呼び出された。IDは" << id_ << " ";
        std::cout << "値: " << value << ", レート: " << rate << std::endl;
    }
};

class IDummy
{
public:
    virtual ~IDummy() = default;
    virtual void DummyFunction(int value, float rate) = 0;
};

class DummyNotEvent : public IDummy
{
public:
    DummyNotEvent()
    {
        std::cout << "DummyNotEventのコンストラクタが呼ばれた。" << std::endl;
    }

    ~DummyNotEvent() override
    {
        std::cout << "DummyNotEventのデストラクタが呼ばれた。" << std::endl;
    }

    void DummyFunction(int value, float rate) override
    {
        std::cout << "DummyNotEventのDummyFunctionが呼び出された。値: " << value << ", レート: " << rate << std::endl;
    }

};
