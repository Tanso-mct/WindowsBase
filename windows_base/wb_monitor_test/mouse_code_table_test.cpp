#include "pch.h"

#include "wb_monitor/include/mouse_code_table.h"
#pragma comment(lib, "wb_monitor.lib")

#include <memory>

TEST(MouseCodeTable, SetUpAndGet)
{
    std::unique_ptr<wb::MouseCodeTable> mouseCodeTable = std::make_unique<wb::MouseCodeTable>();

    mouseCodeTable->SetUpTable();
    ASSERT_TRUE(mouseCodeTable->IsSetUp());

    // XButtonを試すためのWPARAMらのモック
    WPARAM notXButton = 0;
    WPARAM xButton1 = (XBUTTON1 << 16) | 0;
    WPARAM xButton2 = (XBUTTON2 << 16) | 0;

    // マウスコードを取得
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

