#include "pch.h"

#include "windows_base/include/container_static.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    class MockElement
    {
    private:
        size_t value_;
    public:
        MockElement(size_t value) : value_(value) {}
        size_t GetValue() const { return value_; }
    };
}

TEST(StaticContainer, Create)
{
    std::unique_ptr<wb::IStaticContainer<MockElement>> container = std::make_unique<wb::StaticContainer<MockElement>>();

    const size_t CONTAINER_SIZE = 5;
    container->Create(CONTAINER_SIZE);
    EXPECT_EQ(container->GetSize(), CONTAINER_SIZE);
}

TEST(StaticContainer, SetAndGet)
{
    std::unique_ptr<wb::IStaticContainer<MockElement>> container = std::make_unique<wb::StaticContainer<MockElement>>();

    const size_t CONTAINER_SIZE = 5;
    container->Create(CONTAINER_SIZE);

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        container->Set(i, std::make_unique<MockElement>(i));
    }

    // Attempting to set an element at an out-of-bounds index should throw an error
    EXPECT_THROW(container->Set(CONTAINER_SIZE, std::make_unique<MockElement>(0)), std::runtime_error);

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        EXPECT_EQ(container->Get(i).GetValue(), i);
    }

    // Accessing out of bounds should throw an error
    EXPECT_THROW(container->Get(CONTAINER_SIZE), std::runtime_error);
}

TEST(StaticContainer, PtrGet)
{
    std::unique_ptr<wb::IStaticContainer<MockElement>> container = std::make_unique<wb::StaticContainer<MockElement>>();

    const size_t CONTAINER_SIZE = 5;
    container->Create(CONTAINER_SIZE);

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        container->Set(i, std::make_unique<MockElement>(i));
    }

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        MockElement *elementPtr = container->PtrGet(i);
        EXPECT_EQ(elementPtr->GetValue(), i);
    }

    // Accessing out of bounds should throw an error
    EXPECT_THROW(container->PtrGet(CONTAINER_SIZE), std::runtime_error);
}

TEST(StaticContainer, ThreadSafeGet)
{
    std::unique_ptr<wb::IStaticContainer<MockElement>> container = std::make_unique<wb::StaticContainer<MockElement>>();

    const size_t CONTAINER_SIZE = 5;
    container->Create(CONTAINER_SIZE);

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        container->Set(i, std::make_unique<MockElement>(i));
    }

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        wb::LockedRef<MockElement> &lockedRef = container->ThreadSafeGet(i);
        EXPECT_EQ(lockedRef().GetValue(), i);
    }

    // Accessing out of bounds should throw an error
    EXPECT_THROW(container->ThreadSafeGet(CONTAINER_SIZE), std::runtime_error);
}

TEST(StaticContainer, Release)
{
    std::unique_ptr<wb::IStaticContainer<MockElement>> container = std::make_unique<wb::StaticContainer<MockElement>>();

    const size_t CONTAINER_SIZE = 5;
    container->Create(CONTAINER_SIZE);

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        container->Set(i, std::make_unique<MockElement>(i));
    }

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        std::unique_ptr<MockElement> releasedElement = container->Release(i);
        EXPECT_EQ(releasedElement->GetValue(), i);
    }

    // Attempting to release an element at an out-of-bounds index should throw an error
    EXPECT_THROW(container->Release(CONTAINER_SIZE), std::runtime_error);

    EXPECT_EQ(container->GetSize(), CONTAINER_SIZE); // Size should remain the same after release
}

TEST(StaticContainer, Clear)
{
    std::unique_ptr<wb::IStaticContainer<MockElement>> container = std::make_unique<wb::StaticContainer<MockElement>>();

    const size_t CONTAINER_SIZE = 5;
    container->Create(CONTAINER_SIZE);
    EXPECT_EQ(container->GetSize(), CONTAINER_SIZE);

    container->Clear();
    EXPECT_EQ(container->GetSize(), 0);
}