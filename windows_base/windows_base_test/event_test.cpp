#include "pch.h"

#include "windows_base/include/event.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    using EVENT_KEY = int;
    using FUNC_KEY = bool;

    constexpr int TEMP_VALUE = 7;

    constexpr int MOCK_EVENT_KEY = 1;
    constexpr int MOCK_EVENT_A_KEY = 1;
    constexpr int MOCK_EVENT_B_KEY = 2;
    
    constexpr bool MOCK_FUNC_KEY = true;
    constexpr bool MOCK_ANOTHER_FUNC_KEY = false;

    class IMockEvent
    {
    public:
        virtual ~IMockEvent() = default;
        virtual void OnEventTriggered(int value) = 0;
        virtual void OnAnotherEventTriggered(int value) = 0;
    };

    class MockAEvent : public IMockEvent
    {
    public:
        MockAEvent() = default;
        ~MockAEvent() = default;

        void OnEventTriggered(int value) override
        {
            // Mock event handling logic
            std::cout << "Event A triggered with value: " << value << std::endl;
        }

        void OnAnotherEventTriggered(int value) override
        {
            // Mock another event handling logic
            std::cout << "Another Event A triggered with value: " << value << std::endl;
        }
    };

    class MockBEvent : public IMockEvent
    {
    public:
        MockBEvent() = default;
        ~MockBEvent() override = default;

        void OnEventTriggered(int value) override
        {
            // Mock event handling logic
            std::cout << "Event B triggered with value: " << value << std::endl;
        }

        void OnAnotherEventTriggered(int value) override
        {
            // Mock another event handling logic
            std::cout << "Another Event B triggered with value: " << value << std::endl;
        }
    };
}

TEST(Event, UseEventInstanceTable)
{
    // Create an instance table for MockAEvent
    wb::EventInstTable<EVENT_KEY, IMockEvent> eventInstanceTable;

    // Add an event instance
    {
        std::unique_ptr<IMockEvent> eventInstance = std::make_unique<MockAEvent>();
        eventInstanceTable.Add(MOCK_EVENT_KEY, std::move(eventInstance));
    }

    // Check if the event instance exists
    EXPECT_TRUE(eventInstanceTable.Has(MOCK_EVENT_KEY));

    // Check the size of the instance table
    EXPECT_EQ(eventInstanceTable.GetSize(), 1);

    // Get the event instance
    {
        std::unique_ptr<IMockEvent> &eventInstance = eventInstanceTable.Get(MOCK_EVENT_KEY);
        EXPECT_NE(eventInstance.get(), nullptr);
    }

    // Set the event B instance
    {
        std::unique_ptr<IMockEvent> eventInstance = std::make_unique<MockBEvent>();
        eventInstanceTable.Set(MOCK_EVENT_KEY, std::move(eventInstance));
    }

    // Get the event instance again
    {
        std::unique_ptr<IMockEvent> &eventInstance = eventInstanceTable.Get(MOCK_EVENT_KEY);
        EXPECT_NE(eventInstance.get(), nullptr);
        
        // Check this event instance's type
        MockBEvent *mockBEvent = dynamic_cast<MockBEvent*>(eventInstance.get());
        EXPECT_NE(mockBEvent, nullptr);
    }

    // Remove the event instance
    {
        std::unique_ptr<IMockEvent> removedEvent = eventInstanceTable.Remove(MOCK_EVENT_KEY);
        EXPECT_NE(removedEvent.get(), nullptr);
    }

    // Add another event instance
    {
        std::unique_ptr<IMockEvent> eventInstance = std::make_unique<MockAEvent>();
        eventInstanceTable.Add(MOCK_EVENT_KEY, std::move(eventInstance));
    }

    // Clear the instance table
    eventInstanceTable.Clear();

    // Check if the instance table is empty
    EXPECT_FALSE(eventInstanceTable.Has(MOCK_EVENT_KEY));

    // Check the size of the instance table
    EXPECT_EQ(eventInstanceTable.GetSize(), 0);
}

TEST(Event, UseEventFuncTable)
{
    // Create a function table for MockAEvent
    wb::EventFuncTable<FUNC_KEY, IMockEvent, int> eventFuncTable;

    // Add a function to the event instance
    eventFuncTable.Add(MOCK_FUNC_KEY, &IMockEvent::OnEventTriggered);

    // Check if the function exists
    EXPECT_TRUE(eventFuncTable.Has(MOCK_FUNC_KEY));

    // Check the size of the function table
    EXPECT_EQ(eventFuncTable.GetSize(), 1);

    // Get the function from the table
    {
        void (IMockEvent::*eventFunc)(int) = eventFuncTable.Get(MOCK_FUNC_KEY);
        EXPECT_NE(eventFunc, nullptr);

        // Create an event instance to invoke the function
        std::unique_ptr<IMockEvent> eventInstance = std::make_unique<MockAEvent>();
        
        // Invoke the function on the event instance
        (eventInstance.get()->*eventFunc)(TEMP_VALUE);
    }

    // Set a new function for the event instance
    eventFuncTable.Set(MOCK_FUNC_KEY, &IMockEvent::OnAnotherEventTriggered);

    // Get the new function from the table
    {
        void (IMockEvent::*eventFunc)(int) = eventFuncTable.Get(MOCK_FUNC_KEY);
        EXPECT_NE(eventFunc, nullptr);

        // Create an event instance to invoke the function
        std::unique_ptr<IMockEvent> eventInstance = std::make_unique<MockAEvent>();
        
        // Invoke the function on the event instance
        (eventInstance.get()->*eventFunc)(TEMP_VALUE);
    }

    // Remove the function from the table
    {
        void (IMockEvent::*removedFunc)(int) = eventFuncTable.Remove(MOCK_FUNC_KEY);
        EXPECT_NE(removedFunc, nullptr);
    }

    // Add the function back to the table
    eventFuncTable.Add(MOCK_FUNC_KEY, &IMockEvent::OnEventTriggered);

    // Clear the function table
    eventFuncTable.Clear();

    // Check if the function table is empty
    EXPECT_FALSE(eventFuncTable.Has(MOCK_FUNC_KEY));

    // Check the size of the function table
    EXPECT_EQ(eventFuncTable.GetSize(), 0);
}

TEST(Event, UseEventInvoker)
{
    // Create an event invoker
    wb::EventInvoker<EVENT_KEY, IMockEvent, FUNC_KEY, int> eventInvoker;

    // Create and set the instance table
    {
        std::unique_ptr<wb::IEventInstanceTable<EVENT_KEY, IMockEvent>> instanceTable 
            =std::make_unique<wb::EventInstTable<EVENT_KEY, IMockEvent>>();

        std::unique_ptr<IMockEvent> eventAInstance = std::make_unique<MockAEvent>();
        instanceTable->Add(MOCK_EVENT_A_KEY, std::move(eventAInstance));

        std::unique_ptr<IMockEvent> eventBInstance = std::make_unique<MockBEvent>();
        instanceTable->Add(MOCK_EVENT_B_KEY, std::move(eventBInstance));

        eventInvoker.SetInstanceTable(std::move(instanceTable));
    }

    // Check if the instance table is set
    EXPECT_NO_THROW(eventInvoker.GetInstanceTable());

    // Create and set the function table
    {
        std::unique_ptr<wb::IEventFuncTable<FUNC_KEY, IMockEvent, int>> funcTable 
            = std::make_unique<wb::EventFuncTable<FUNC_KEY, IMockEvent, int>>();

        funcTable->Add(MOCK_FUNC_KEY, &IMockEvent::OnEventTriggered);
        funcTable->Add(MOCK_ANOTHER_FUNC_KEY, &IMockEvent::OnAnotherEventTriggered);

        eventInvoker.SetFuncTable(std::move(funcTable));
    }

    // Check if the function table is set
    EXPECT_NO_THROW(eventInvoker.GetFuncTable());

    // Invoke the event A with the function
    EXPECT_NO_THROW(eventInvoker.Invoke(MOCK_EVENT_A_KEY, MOCK_FUNC_KEY, TEMP_VALUE));

    // Invoke the event B with another function
    EXPECT_NO_THROW(eventInvoker.Invoke(MOCK_EVENT_B_KEY, MOCK_ANOTHER_FUNC_KEY, TEMP_VALUE));
}