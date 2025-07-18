#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/monitor_factory_registry.h"
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

        const size_t &GetFactoryID() const override
        {
            return MockMonitorFactoryID();
        }

        void EditState(UINT msg, WPARAM wParam, LPARAM lParam) override
        {
            // Mock implementation for testing purposes
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

    class MockMonitorFactory : public wb::IMonitorFactory
    {
    public:
        MockMonitorFactory() = default;
        virtual ~MockMonitorFactory() override = default;

        std::unique_ptr<wb::IMonitor> Create() const override
        {
            return std::make_unique<MockMonitor>();
        }
    };

    WB_REGISTER_MONITOR_FACTORY(MockMonitorFactory, MockMonitorFactoryID());
}

TEST(MonitorFactoryRegistry, GetFactory)
{
    wb::IMonitorFactory &factory = wb::gMonitorFactoryRegistry.GetFactory(MockMonitorFactoryID());
    EXPECT_NE(&factory, nullptr);
}
