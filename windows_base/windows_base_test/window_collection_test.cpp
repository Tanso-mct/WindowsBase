#include "pch.h"

#include "windows_base/include/interfaces/factory.h"
#include "windows_base/include/id_factory.h"
#include "windows_base/include/window_collection.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    /*******************************************************************************************************************
     * Mock Window ID
    /******************************************************************************************************************/

    const size_t &MockWindowID()
    {
        static size_t id = wb::IDFactory::CreateWindowID();
        return id;
    }

    /*******************************************************************************************************************
     * Mock Window Facade Factory
    /******************************************************************************************************************/

    class MockWindowFacadeFactory : public wb::IWindowFacadeFactory
    {
    public:
        MockWindowFacadeFactory() = default;
        ~MockWindowFacadeFactory() override = default;

        std::unique_ptr<wb::IWindowFacade> Create() const override
        {
            return nullptr;
        }
    };

    /*******************************************************************************************************************
     * Mock Window Event Factory
    /******************************************************************************************************************/

    class MockWindowEventFactory : public wb::IWindowEventFactory
    {
    public:
        MockWindowEventFactory() = default;
        ~MockWindowEventFactory() override = default;

        std::unique_ptr<wb::IWindowEvent> Create() const override
        {
            return nullptr;
        }
    };

    /*******************************************************************************************************************
     * Register Mock Window
    /******************************************************************************************************************/

    WB_REGISTER_WINDOW(MockWindowID, MockWindowFacadeFactory, MockWindowEventFactory)    
}

TEST(WindowCollection, GetFactories)
{
    // Get the facade factory
    wb::IWindowFacadeFactory &facadeFactory = wb::gWindowCollection.GetFacadeFactory(MockWindowID());
    EXPECT_NE(&facadeFactory, nullptr);

    // Get the event factory
    wb::IWindowEventFactory &eventFactory = wb::gWindowCollection.GetEventFactory(MockWindowID());
    EXPECT_NE(&eventFactory, nullptr);
}