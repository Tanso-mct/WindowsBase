#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/monitor_collection.h"
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
     * Mock Monitor ID
    /******************************************************************************************************************/

    const size_t &MockMonitorID()
    {
        static size_t id = wb::IDFactory::CreateMonitorID();
        return id;
    }

    /*******************************************************************************************************************
     * Register Mock Monitor
    /******************************************************************************************************************/

    WB_REGISTER_MONITOR(MockMonitorID, MockMonitorFactoryID());
}

TEST(MonitorCollection, GetFactoryID)
{
    // Get the factory ID for the mock monitor
    const size_t &factoryID = wb::gMonitorCollection.GetFactoryID(MockMonitorID());
    EXPECT_EQ(factoryID, MockMonitorFactoryID());
}