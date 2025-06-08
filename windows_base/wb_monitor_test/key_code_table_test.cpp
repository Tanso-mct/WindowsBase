#include "pch.h"

#include "wb_monitor/include/key_code_table.h"
#pragma comment(lib, "wb_monitor.lib")

#include <memory>

TEST(KeyCodeTable, SetUpAndGet)
{
    std::unique_ptr<wb::IKeyCodeTable> keyCodeTable = std::make_unique<wb::KeyCodeTable>();
    keyCodeTable->SetUpTable();

    EXPECT_TRUE(keyCodeTable->IsSetUp());

    // 通常キーとしてのLPARAM
    LPARAM normalLParam = 0;

    // 拡張キーとしてのLPARAM
    LPARAM extendedLParam = 0;
    extendedLParam |= (1 << 24); // 右側のキーを示すビットをセット

    // Check if specific key codes are correctly mapped
    EXPECT_EQ(keyCodeTable->GetKeyCode('A', normalLParam), wb::KeyCode::A);
    EXPECT_EQ(keyCodeTable->GetKeyCode(VK_RETURN, normalLParam), wb::KeyCode::Return);
    EXPECT_EQ(keyCodeTable->GetKeyCode(VK_MENU, extendedLParam), wb::KeyCode::RAlt);
}
