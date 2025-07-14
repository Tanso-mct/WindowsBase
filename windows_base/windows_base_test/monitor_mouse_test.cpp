#include "pch.h"

#include "windows_base/include/monitor_mouse.h"
#include "windows_base/include/monitor_factory_collection.h"
#include "windows_base/include/id_factory.h"
#include "windows_base/include/monitor_collection.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    /*******************************************************************************************************************
     * Mock Mouse Monitor ID
    /******************************************************************************************************************/

    const size_t &MockMouseMonitorID()
    {
        static size_t id = wb::IDFactory::CreateMonitorID();
        return id;
    }

    /*******************************************************************************************************************
     * Register Mock Mouse Monitor
    /******************************************************************************************************************/

    WB_REGISTER_MONITOR(MockMouseMonitorID(), wb::DefaultMouseMonitorFactoryID());
}

TEST(mouseMonitor, UseInputTypeTable)
{
    std::unique_ptr<wb::IInputTypeTable> inputTypeTable = std::make_unique<wb::MouseInputTypeTable>();

    inputTypeTable->Create();
    EXPECT_TRUE(inputTypeTable->CheckIsReady());

    // Test input type retrieval
    EXPECT_EQ(inputTypeTable->GetInputType(WM_LBUTTONDOWN), wb::INPUT_DOWN);
    EXPECT_EQ(inputTypeTable->GetInputType(WM_LBUTTONUP), wb::INPUT_UP);
    EXPECT_EQ(inputTypeTable->GetInputType(WM_RBUTTONDOWN), wb::INPUT_DOWN);
    EXPECT_EQ(inputTypeTable->GetInputType(WM_RBUTTONUP), wb::INPUT_UP);
}

TEST(MouseMonitor, UseMouseCodeTable)
{
    std::unique_ptr<wb::IMouseCodeTable> mouseCodeTable = std::make_unique<wb::MouseCodeTable>();

    mouseCodeTable->Create();
    EXPECT_TRUE(mouseCodeTable->CheckIsReady());

    // Test X button codes
    WPARAM notXButton = 0;
    WPARAM xButton1 = (XBUTTON1 << 16) | 0;
    WPARAM xButton2 = (XBUTTON2 << 16) | 0;

    // Test mouse code retrieval
    EXPECT_EQ(mouseCodeTable->GetMouseCode(WM_LBUTTONDOWN, notXButton), wb::MouseCode::Left);
    EXPECT_EQ(mouseCodeTable->GetMouseCode(WM_LBUTTONUP, notXButton), wb::MouseCode::Left);
    EXPECT_EQ(mouseCodeTable->GetMouseCode(WM_RBUTTONDOWN, notXButton), wb::MouseCode::Right);
    EXPECT_EQ(mouseCodeTable->GetMouseCode(WM_RBUTTONUP, notXButton), wb::MouseCode::Right);
    EXPECT_EQ(mouseCodeTable->GetMouseCode(WM_MBUTTONDOWN, notXButton), wb::MouseCode::Middle);
    EXPECT_EQ(mouseCodeTable->GetMouseCode(WM_MBUTTONUP, notXButton), wb::MouseCode::Middle);

    EXPECT_EQ(mouseCodeTable->GetMouseCode(WM_XBUTTONDOWN, xButton1), wb::MouseCode::X1);
    EXPECT_EQ(mouseCodeTable->GetMouseCode(WM_XBUTTONUP, xButton1), wb::MouseCode::X1);
    EXPECT_EQ(mouseCodeTable->GetMouseCode(WM_XBUTTONDOWN, xButton2), wb::MouseCode::X2);
    EXPECT_EQ(mouseCodeTable->GetMouseCode(WM_XBUTTONUP, xButton2), wb::MouseCode::X2);
}

TEST(MouseMonitor, Create)
{
    // Get the monitor factory id
    const size_t &factoryID = wb::gMonitorCollection.GetFactoryID(MockMouseMonitorID());
    EXPECT_EQ(factoryID, wb::DefaultMouseMonitorFactoryID());

    // Get the monitor factory
    wb::IMonitorFactory &monitorFactory = wb::gMonitorFactoryCollection.GetFactory(factoryID);
    EXPECT_NE(&monitorFactory, nullptr);

    // Create a monitor using the factory
    std::unique_ptr<wb::IMonitor> monitor = monitorFactory.Create();
    EXPECT_NE(monitor, nullptr);
    EXPECT_EQ(monitor->GetFactoryID(), wb::DefaultMouseMonitorFactoryID());
}

TEST(MouseMonitor, Use)
{
    // Get the monitor factory id
    const size_t &factoryID = wb::gMonitorCollection.GetFactoryID(MockMouseMonitorID());
    EXPECT_EQ(factoryID, wb::DefaultMouseMonitorFactoryID());

    // Get the monitor factory
    wb::IMonitorFactory &monitorFactory = wb::gMonitorFactoryCollection.GetFactory(factoryID);
    EXPECT_NE(&monitorFactory, nullptr);

    // Create a monitor using the factory
    std::unique_ptr<wb::IMonitor> monitor = monitorFactory.Create();
    EXPECT_NE(monitor, nullptr);
    EXPECT_EQ(monitor->GetFactoryID(), wb::DefaultMouseMonitorFactoryID());

    wb::IMouseMonitor *mouseMonitor = dynamic_cast<wb::IMouseMonitor*>(monitor.get());
    EXPECT_NE(mouseMonitor, nullptr);

    WPARAM notXButton = 0;
    LPARAM mockLParam = 0;

    // Down, Up, DoubleTap test
    mouseMonitor->EditState(WM_LBUTTONDOWN, notXButton, mockLParam);
    EXPECT_TRUE(mouseMonitor->GetButton(wb::MouseCode::Left));
    EXPECT_TRUE(mouseMonitor->GetButtonDown(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonUp(wb::MouseCode::Left));

    mouseMonitor->UpdateState();
    EXPECT_TRUE(mouseMonitor->GetButton(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonDown(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonUp(wb::MouseCode::Left));

    mouseMonitor->EditState(WM_LBUTTONUP, notXButton, mockLParam);
    EXPECT_FALSE(mouseMonitor->GetButton(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonDown(wb::MouseCode::Left));
    EXPECT_TRUE(mouseMonitor->GetButtonUp(wb::MouseCode::Left));

    mouseMonitor->UpdateState();
    EXPECT_FALSE(mouseMonitor->GetButton(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonDown(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonUp(wb::MouseCode::Left));

    mouseMonitor->EditState(WM_LBUTTONDOWN, notXButton, mockLParam);
    mouseMonitor->UpdateState();

    mouseMonitor->EditState(WM_LBUTTONUP, notXButton, mockLParam);
    EXPECT_TRUE(mouseMonitor->GetButtonDoubleTap(wb::MouseCode::Left, 0.5));

    mouseMonitor->UpdateState();
}

