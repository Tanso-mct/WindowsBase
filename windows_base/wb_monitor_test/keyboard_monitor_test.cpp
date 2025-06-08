#include "pch.h"

#include "wb_monitor/include/keyboard_monitor.h"
#include "wb_monitor/include/keyboard_context.h"
#include "wb_monitor/include/key_code_table.h"
#pragma comment(lib, "wb_monitor.lib")

TEST(KeyboardMonitor, SetUpAndEdit)
{
    std::unique_ptr<wb::IKeyboardMonitor> keyboardMonitor = std::make_unique<wb::KeyboardMonitor>();

    {
        std::unique_ptr<wb::IKeyboardContext> context = std::make_unique<wb::KeyboardContext>();
        keyboardMonitor->SetUpContext(std::move(context));
    }

    {
        std::unique_ptr<wb::IKeyCodeTable> keyCodeTable = std::make_unique<wb::KeyCodeTable>();
        keyboardMonitor->SetUpCodeTable(std::move(keyCodeTable));
    }

    EXPECT_TRUE(keyboardMonitor->IsSetUp());

    // Down, Up, DoubleTapのテスト
    keyboardMonitor->EditStateByDown('A', 0);
    EXPECT_TRUE(keyboardMonitor->GetKey(wb::KeyCode::A));
    EXPECT_TRUE(keyboardMonitor->GetKeyDown(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyUp(wb::KeyCode::A));

    keyboardMonitor->UpdateState();
    EXPECT_TRUE(keyboardMonitor->GetKey(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyDown(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyUp(wb::KeyCode::A));

    keyboardMonitor->EditStateByUp('A', 0);
    EXPECT_FALSE(keyboardMonitor->GetKey(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyDown(wb::KeyCode::A));
    EXPECT_TRUE(keyboardMonitor->GetKeyUp(wb::KeyCode::A));

    keyboardMonitor->UpdateState();
    EXPECT_FALSE(keyboardMonitor->GetKey(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyDown(wb::KeyCode::A));
    EXPECT_FALSE(keyboardMonitor->GetKeyUp(wb::KeyCode::A));

    keyboardMonitor->EditStateByDown('A', 0);
    keyboardMonitor->UpdateState();

    keyboardMonitor->EditStateByUp('A', 0);
    EXPECT_TRUE(keyboardMonitor->GetKeyDoubleTap(wb::KeyCode::A, 0.5));

    keyboardMonitor->UpdateState();
}