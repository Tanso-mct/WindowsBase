#include "pch.h"

#include "wb_monitor/include/mouse_input_type_table.h"
#pragma comment(lib, "wb_monitor.lib")

#include <memory>

TEST(MouseInputTypeTable, SetUpAndGet)
{
    std::unique_ptr<wb::MouseInputTypeTable> mouseInputTypeTable = std::make_unique<wb::MouseInputTypeTable>();

    mouseInputTypeTable->SetUpTable();
    ASSERT_TRUE(mouseInputTypeTable->IsSetUp());

    // マウスの入力タイプを取得
    EXPECT_EQ(mouseInputTypeTable->GetInputType(WM_LBUTTONDOWN), wb::INPUT_DOWN);
    EXPECT_EQ(mouseInputTypeTable->GetInputType(WM_LBUTTONUP), wb::INPUT_UP);

    EXPECT_EQ(mouseInputTypeTable->GetInputType(WM_RBUTTONDOWN), wb::INPUT_DOWN);
    EXPECT_EQ(mouseInputTypeTable->GetInputType(WM_RBUTTONUP), wb::INPUT_UP);

    EXPECT_EQ(mouseInputTypeTable->GetInputType(WM_MBUTTONDOWN), wb::INPUT_DOWN);
    EXPECT_EQ(mouseInputTypeTable->GetInputType(WM_MBUTTONUP), wb::INPUT_UP);

    EXPECT_EQ(mouseInputTypeTable->GetInputType(WM_XBUTTONDOWN), wb::INPUT_DOWN);
    EXPECT_EQ(mouseInputTypeTable->GetInputType(WM_XBUTTONUP), wb::INPUT_UP);
}