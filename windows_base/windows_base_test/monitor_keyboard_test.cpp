#include "pch.h"

#include "windows_base/include/monitor_keyboard.h"
#include "windows_base/include/monitor_factory_collection.h"
#include "windows_base/include/id_factory.h"
#include "windows_base/include/monitor_collection.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    /*******************************************************************************************************************
     * Mock Keyboard Monitor ID
    /******************************************************************************************************************/

    const size_t &MockKeyboardMonitorID()
    {
        static size_t id = wb::IDFactory::CreateMonitorID();
        return id;
    }

    /*******************************************************************************************************************
     * Register Mock Keyboard Monitor
    /******************************************************************************************************************/

    WB_REGISTER_MONITOR(MockKeyboardMonitorID(), wb::DefaultKeyboardMonitorFactoryID());
}

TEST(keyboardMonitor, UseInputTypeTable)
{
    std::unique_ptr<wb::IInputTypeTable> inputTypeTable = std::make_unique<wb::KeyboardInputTypeTable>();

    inputTypeTable->Create();
    EXPECT_TRUE(inputTypeTable->CheckIsReady());

    // Test input type retrieval
    EXPECT_EQ(inputTypeTable->GetInputType(WM_KEYDOWN), wb::INPUT_DOWN);
    EXPECT_EQ(inputTypeTable->GetInputType(WM_KEYUP), wb::INPUT_UP);
    EXPECT_EQ(inputTypeTable->GetInputType(WM_SYSKEYDOWN), wb::INPUT_DOWN);
    EXPECT_EQ(inputTypeTable->GetInputType(WM_SYSKEYUP), wb::INPUT_UP);
}

TEST(KeyboardMonitor, UseKeyCodeTable)
{
    std::unique_ptr<wb::IKeyCodeTable> keyCodeTable = std::make_unique<wb::KeyCodeTable>();

    keyCodeTable->Create();
    EXPECT_TRUE(keyCodeTable->CheckIsReady());

    // Normal lparam
    LPARAM normalLParam = 0;

    // Extended lparam
    LPARAM extendedLParam = 0;
    extendedLParam |= (1 << 24); // Set the extended key flag

    // Test key code retrieval
    EXPECT_EQ(keyCodeTable->GetKeyCode('A', normalLParam), wb::KeyCode::A);
    EXPECT_EQ(keyCodeTable->GetKeyCode(VK_RETURN, normalLParam), wb::KeyCode::Return);
    EXPECT_EQ(keyCodeTable->GetKeyCode(VK_MENU, extendedLParam), wb::KeyCode::RAlt);
}

TEST(KeyboardMonitor, Create)
{
    // Get the monitor factory id
    const size_t &factoryID = wb::gMonitorCollection.GetFactoryID(MockKeyboardMonitorID());
    EXPECT_EQ(factoryID, wb::DefaultKeyboardMonitorFactoryID());

    // Get the monitor factory
    wb::IMonitorFactory &factory = wb::gMonitorFactoryCollection.GetFactory(factoryID);
    EXPECT_NE(&factory, nullptr);

    // Create a monitor using the factory
    std::unique_ptr<wb::IMonitor> monitor = factory.Create();
    EXPECT_NE(monitor, nullptr);
    EXPECT_EQ(monitor->GetFactoryID(), wb::DefaultKeyboardMonitorFactoryID());
}

TEST(KeyboardMonitor, Use)
{
    // Get the monitor factory id
    const size_t &factoryID = wb::gMonitorCollection.GetFactoryID(MockKeyboardMonitorID());
    EXPECT_EQ(factoryID, wb::DefaultKeyboardMonitorFactoryID());

    // Get the monitor factory
    wb::IMonitorFactory &factory = wb::gMonitorFactoryCollection.GetFactory(factoryID);
    EXPECT_NE(&factory, nullptr);

    // Create a monitor using the factory
    std::unique_ptr<wb::IMonitor> monitor = factory.Create();
    EXPECT_EQ(monitor->GetFactoryID(), wb::DefaultKeyboardMonitorFactoryID());

    wb::DefaultKeyboardMonitor *keyboardMonitor = dynamic_cast<wb::DefaultKeyboardMonitor*>(monitor.get());
    EXPECT_NE(keyboardMonitor, nullptr);

    // Down, Up, DoubleTap test
    keyboardMonitor->EditState(WM_KEYDOWN, 'A', 0);
    EXPECT_TRUE(keyboardMonitor->GetKey(wb::KeyCode::A));
    EXPECT_TRUE(keyboardMonitor->GetKeyDown(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyUp(wb::KeyCode::A));

    keyboardMonitor->UpdateState();
    EXPECT_TRUE(keyboardMonitor->GetKey(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyDown(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyUp(wb::KeyCode::A));

    keyboardMonitor->EditState(WM_KEYUP, 'A', 0);
    EXPECT_FALSE(keyboardMonitor->GetKey(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyDown(wb::KeyCode::A));
    EXPECT_TRUE(keyboardMonitor->GetKeyUp(wb::KeyCode::A));

    keyboardMonitor->UpdateState();
    EXPECT_FALSE(keyboardMonitor->GetKey(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyDown(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyUp(wb::KeyCode::A));

    keyboardMonitor->EditState(WM_KEYDOWN, 'A', 0);
    keyboardMonitor->UpdateState();

    keyboardMonitor->EditState(WM_KEYUP, 'A', 0);
    EXPECT_TRUE(keyboardMonitor->GetKeyDoubleTap(wb::KeyCode::A, 0.5));

    keyboardMonitor->UpdateState();
}