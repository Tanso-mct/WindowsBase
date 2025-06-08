#include "pch.h"

#include "wb_event/include/event_inst_table.h"

#include "wb_event_test/dummy.h"

TEST(EventInstTable, AddAndGet)
{
    std::unique_ptr<wb::IEventInstTable<int, DummyEvent>> eventTable
    = std::make_unique<wb::EventInstTable<int, DummyEvent>>();

    int key = 1;
    const int dummyId = 7;
    {
        std::unique_ptr<DummyEvent> dummyEvent = std::make_unique<DummyEvent>(dummyId);
        eventTable->Add(key, std::move(dummyEvent));
    }

    EXPECT_EQ(eventTable->GetSize(), 1);
    EXPECT_TRUE(eventTable->Has(key));

    {
        std::unique_ptr<DummyEvent>& dummyEvent = eventTable->Get(key);
        EXPECT_NE(dummyEvent, nullptr);
        dummyEvent->IsDestroying();
    }
}

// TEST(EventInstTable, AddNotEvent)
// {
//     std::unique_ptr<wb::IEventInstTable<int, DummyNotEvent>> eventTable
//     = std::make_unique<wb::EventInstTable<int, DummyNotEvent>>();

//     int key = 1;
//     {
//         std::unique_ptr<DummyNotEvent> dummyNotEvent = std::make_unique<DummyNotEvent>();
//         eventTable->Add(key, std::move(dummyNotEvent)); // Error
//     }
// }

TEST(EventInstTable, Remove)
{
    std::unique_ptr<wb::IEventInstTable<int, DummyEvent>> eventTable
    = std::make_unique<wb::EventInstTable<int, DummyEvent>>();

    int key = 1;
    const int dummyId = 7;
    {
        std::unique_ptr<DummyEvent> dummyEvent = std::make_unique<DummyEvent>(dummyId);
        eventTable->Add(key, std::move(dummyEvent));
    }

    EXPECT_EQ(eventTable->GetSize(), 1);
    EXPECT_TRUE(eventTable->Has(key));

    {
        std::unique_ptr<DummyEvent> removedEvent = eventTable->Remove(key);
        EXPECT_NE(removedEvent, nullptr);
        removedEvent->IsDestroying();
    }

    EXPECT_EQ(eventTable->GetSize(), 0);
    EXPECT_FALSE(eventTable->Has(key));
}

TEST(EventInstTable, Set)
{
    std::unique_ptr<wb::IEventInstTable<int, DummyEvent>> eventTable
    = std::make_unique<wb::EventInstTable<int, DummyEvent>>();

    int key = 1;
    const int dummyId = 7;
    {
        std::unique_ptr<DummyEvent> dummyEvent = std::make_unique<DummyEvent>(dummyId);
        eventTable->Add(key, std::move(dummyEvent));
    }

    EXPECT_EQ(eventTable->GetSize(), 1);
    EXPECT_TRUE(eventTable->Has(key));

    const int newDummyId = 100;
    {
        std::unique_ptr<DummyEvent> newDummyEvent = std::make_unique<DummyEvent>(newDummyId);
        eventTable->Set(key, std::move(newDummyEvent));
    }

    {
        std::unique_ptr<DummyEvent>& dummyEvent = eventTable->Get(key);
        EXPECT_NE(dummyEvent, nullptr);
        dummyEvent->IsDestroying();
    }
}

TEST(EventInstTable, Clear)
{
    std::unique_ptr<wb::IEventInstTable<int, DummyEvent>> eventTable
    = std::make_unique<wb::EventInstTable<int, DummyEvent>>();

    int key = 1;
    const int dummyId = 7;
    {
        std::unique_ptr<DummyEvent> dummyEvent = std::make_unique<DummyEvent>(dummyId);
        eventTable->Add(key, std::move(dummyEvent));
    }

    EXPECT_EQ(eventTable->GetSize(), 1);
    EXPECT_TRUE(eventTable->Has(key));

    eventTable->Clear();

    EXPECT_EQ(eventTable->GetSize(), 0);
    EXPECT_FALSE(eventTable->Has(key));
}