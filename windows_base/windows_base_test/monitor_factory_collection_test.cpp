#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/interfaces/monitor.h"
#include "windows_base/include/monitor_factory_collection.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    /*******************************************************************************************************************
     * Mock Monitor Factory ID
    /******************************************************************************************************************/

    const size_t &MockMonitorFactoryID()
    {
        static size_t id = wb::IDFactory::CreateMonitorFactoryID();
        return id;
    }

    /*******************************************************************************************************************
     * Mock Monitor Factory
     * This factory creates a mock monitor for testing purposes.
    /******************************************************************************************************************/

    class MockMonitor : public wb::IMonitor
    {
    public:
        MockMonitor() = default;
        virtual ~MockMonitor() override = default;

        /*******************************************************************************************************************
         * IFacade implementation
        /******************************************************************************************************************/

        void SetContext(std::unique_ptr<wb::IContext> context) override
        {
            // Mock implementation for testing purposes
        }

        bool CheckIsReady() const override
        {
            // Mock implementation for testing purposes
            return true;
        }

        /*******************************************************************************************************************
         * IMonitor implementation
        /******************************************************************************************************************/

        const size_t &GetID() const override
        {
            return MockMonitorFactoryID();
        }

        void UpdateState() override
        {
            // Mock implementation for testing purposes
        }

        void UpdateRawInputState(RAWINPUT* raw) override
        {
            // Mock implementation for testing purposes
        }
    };

    WB_REGISTER_MONITOR_FACTORY(MockMonitor, MockMonitorFactoryID());
}

TEST(MonitorFactoryCollection, GetFactory)
{
    wb::IMonitorFactory &factory = wb::gMonitorFactoryCollection.GetFactory(MockMonitorFactoryID());
    EXPECT_NE(&factory, nullptr);
}
