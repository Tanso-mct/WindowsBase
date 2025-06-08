#include "pch.h"

#include "wb_event/include/event_func_table.h"

#include "wb_event_test/dummy.h"

#include <memory>

TEST(EventFuncTable, AddAndGet)
{
    std::unique_ptr<wb::IEventFuncTable<int, DummyEvent, int, float>> eventFuncTable
    = std::make_unique<wb::EventFuncTable<int, DummyEvent, int, float>>();

    int key = 1;
    {
        void (DummyEvent::*dummyFunc)(int, float) = &DummyEvent::DummyFunction;
        eventFuncTable->Add(key, dummyFunc);
    }

    EXPECT_EQ(eventFuncTable->GetSize(), 1);
    EXPECT_TRUE(eventFuncTable->Has(key));

    {
        void (DummyEvent::*dummyFunc)(int, float) = eventFuncTable->Get(key);
        EXPECT_NE(dummyFunc, nullptr);

        const int dummyId = 7;
        std::unique_ptr<DummyEvent> dummyEvent = std::make_unique<DummyEvent>(dummyId);
        (dummyEvent.get()->*dummyFunc)(7, 3.14f);
    }
}

TEST(EventFuncTable, Remove)
{
    std::unique_ptr<wb::IEventFuncTable<int, DummyEvent, int, float>> eventFuncTable
    = std::make_unique<wb::EventFuncTable<int, DummyEvent, int, float>>();

    int key = 1;
    {
        void (DummyEvent::*dummyFunc)(int, float) = &DummyEvent::DummyFunction;
        eventFuncTable->Add(key, dummyFunc);
    }

    EXPECT_EQ(eventFuncTable->GetSize(), 1);
    EXPECT_TRUE(eventFuncTable->Has(key));

    {
        void (DummyEvent::*removedFunc)(int, float) = eventFuncTable->Remove(key);
        EXPECT_NE(removedFunc, nullptr);

        const int dummyId = 7;
        std::unique_ptr<DummyEvent> dummyEvent = std::make_unique<DummyEvent>(dummyId);
        (dummyEvent.get()->*removedFunc)(7, 3.14f);
    }

    EXPECT_EQ(eventFuncTable->GetSize(), 0);
    EXPECT_FALSE(eventFuncTable->Has(key));
}

TEST(EventFuncTable, Set)
{
    std::unique_ptr<wb::IEventFuncTable<int, DummyEvent, int, float>> eventFuncTable
    = std::make_unique<wb::EventFuncTable<int, DummyEvent, int, float>>();

    int key = 1;
    {
        void (DummyEvent::*dummyFunc)(int, float) = &DummyEvent::DummyFunction;
        eventFuncTable->Add(key, dummyFunc);
    }

    EXPECT_EQ(eventFuncTable->GetSize(), 1);
    EXPECT_TRUE(eventFuncTable->Has(key));

    {
        void (DummyEvent::*dummyFunc)(int, float) = &DummyEvent::OtherFunction;
        eventFuncTable->Set(key, dummyFunc);
    }

    {
        void (DummyEvent::*dummyFunc)(int, float) = eventFuncTable->Get(key);
        EXPECT_NE(dummyFunc, nullptr);

        const int dummyId = 7;
        std::unique_ptr<DummyEvent> dummyEvent = std::make_unique<DummyEvent>(dummyId);
        (dummyEvent.get()->*dummyFunc)(7, 3.14f);
    }
}

TEST(EventFuncTable, Clear)
{
    std::unique_ptr<wb::IEventFuncTable<int, DummyEvent, int, float>> eventFuncTable
    = std::make_unique<wb::EventFuncTable<int, DummyEvent, int, float>>();

    int key1 = 1;
    int key2 = 2;
    {
        void (DummyEvent::*dummyFunc)(int, float) = &DummyEvent::DummyFunction;
        eventFuncTable->Add(key1, dummyFunc);
        eventFuncTable->Add(key2, dummyFunc);
    }

    EXPECT_EQ(eventFuncTable->GetSize(), 2);
    EXPECT_TRUE(eventFuncTable->Has(key1));
    EXPECT_TRUE(eventFuncTable->Has(key2));

    eventFuncTable->Clear();

    EXPECT_EQ(eventFuncTable->GetSize(), 0);
    EXPECT_FALSE(eventFuncTable->Has(key1));
    EXPECT_FALSE(eventFuncTable->Has(key2));
}
