#include "pch.h"

#include "windows_base/include/container_dynamic.h"
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

TEST(DynamicContainer, AddAndGet)
{
    std::unique_ptr<wb::IDynamicContainer<MockElement>> container 
        = std::make_unique<wb::DynamicContainer<MockElement>>();

    const size_t CONTAINER_SIZE = 5;
    std::vector<std::unique_ptr<wb::OptionalValue>> indices;
    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        std::unique_ptr<wb::OptionalValue> index = container->Add(std::make_unique<MockElement>(i));
        indices.emplace_back(std::move(index));
    }
    EXPECT_EQ(container->GetSize(), CONTAINER_SIZE);

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        MockElement &element = container->Get(*indices[i]);
        EXPECT_EQ(element.GetValue(), i);
    }

    // Accessing out of bounds should throw an error
    std::shared_ptr<size_t> outOfBoundsIndex = std::make_shared<size_t>(CONTAINER_SIZE);
    wb::OptionalValue outOfBoundsValue(outOfBoundsIndex);
    EXPECT_THROW(container->Get(outOfBoundsValue), std::runtime_error);
}

TEST(DynamicContainer, PtrGet)
{
    std::unique_ptr<wb::IDynamicContainer<MockElement>> container 
        = std::make_unique<wb::DynamicContainer<MockElement>>();

    const size_t CONTAINER_SIZE = 5;
    std::vector<std::unique_ptr<wb::OptionalValue>> indices;
    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        std::unique_ptr<wb::OptionalValue> index = container->Add(std::make_unique<MockElement>(i));
        indices.emplace_back(std::move(index));
    }

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        MockElement *elementPtr = container->PtrGet(*indices[i]);
        EXPECT_EQ(elementPtr->GetValue(), i);
    }

    // Accessing out of bounds should throw an error
    std::shared_ptr<size_t> outOfBoundsIndex = std::make_shared<size_t>(CONTAINER_SIZE);
    wb::OptionalValue outOfBoundsValue(outOfBoundsIndex);
    EXPECT_THROW(container->PtrGet(outOfBoundsValue), std::runtime_error);
}

TEST(DynamicContainer, ThreadSafeGet)
{
    std::unique_ptr<wb::IDynamicContainer<MockElement>> container 
        = std::make_unique<wb::DynamicContainer<MockElement>>();

    const size_t CONTAINER_SIZE = 5;
    std::vector<std::unique_ptr<wb::OptionalValue>> indices;
    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        std::unique_ptr<wb::OptionalValue> index = container->Add(std::make_unique<MockElement>(i));
        indices.emplace_back(std::move(index));
    }

    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        wb::LockedRef<MockElement> lockedRef = container->ThreadSafeGet(*indices[i]);
        EXPECT_EQ(lockedRef().GetValue(), i);
    }

    // Accessing out of bounds should throw an error
    std::shared_ptr<size_t> outOfBoundsIndex = std::make_shared<size_t>(CONTAINER_SIZE);
    wb::OptionalValue outOfBoundsValue(outOfBoundsIndex);
    EXPECT_THROW(container->ThreadSafeGet(outOfBoundsValue), std::runtime_error);
}

TEST(DynamicContainer, Erase)
{
    std::unique_ptr<wb::IDynamicContainer<MockElement>> container 
        = std::make_unique<wb::DynamicContainer<MockElement>>();

    const size_t CONTAINER_SIZE = 5;
    std::vector<std::unique_ptr<wb::OptionalValue>> indices;
    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        std::unique_ptr<wb::OptionalValue> index = container->Add(std::make_unique<MockElement>(i));
        indices.emplace_back(std::move(index));
    }

    // Erase an element
    const size_t ERASE_INDEX = 2;
    std::unique_ptr<MockElement> erasedElement = container->Erase(*indices[ERASE_INDEX]);
    EXPECT_EQ(erasedElement->GetValue(), 2);

    // Accessing the erased element should throw an error
    EXPECT_THROW(container->Get(*indices[ERASE_INDEX]), std::runtime_error);

    // Verify the remaining elements
    for (size_t i = 0; i < CONTAINER_SIZE; i++)
    {
        if (i == ERASE_INDEX) continue; // Skip the erased index

        MockElement &element = container->Get(*indices[i]);
        EXPECT_EQ(element.GetValue(), i);
    }

    // Verify the size after erase
    EXPECT_EQ(container->GetSize(), CONTAINER_SIZE - 1);
}