#include "pch.h"

#include "wb_monitor/include/mouse_monitor.h"
#include "wb_monitor/include/mouse_context.h"
#include "wb_monitor/include/mouse_code_table.h"
#include "wb_monitor/include/mouse_input_type_table.h"
#pragma comment(lib, "wb_monitor.lib")

TEST(MouseMonitor, SetUpAndEdit)
{
    std::unique_ptr<wb::IMouseMonitor> mouseMonitor = std::make_unique<wb::MouseMonitor>();

    // MouseContextのセットアップ
    {
        std::unique_ptr<wb::IMouseContext> context = std::make_unique<wb::MouseContext>();
        mouseMonitor->SetUpContext(std::move(context));
    }

    // MouseCodeTableのセットアップ
    {
        std::unique_ptr<wb::IMouseCodeTable> mouseCodeTable = std::make_unique<wb::MouseCodeTable>();
        mouseMonitor->SetUpCodeTable(std::move(mouseCodeTable));
    }

    // MouseInputTypeTableのセットアップ
    {
        std::unique_ptr<wb::IInputTypeTable> inputTypeTable = std::make_unique<wb::MouseInputTypeTable>();
        mouseMonitor->SetUpInputTypeTable(std::move(inputTypeTable));
    }

    EXPECT_TRUE(mouseMonitor->IsSetUp());

    // Done, Up, DoubleTapのテスト
    mouseMonitor->EditState(WM_LBUTTONDOWN, 0, 0);
    EXPECT_TRUE(mouseMonitor->GetButton(wb::MouseCode::Left));
    EXPECT_TRUE(mouseMonitor->GetButtonDown(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonUp(wb::MouseCode::Left));

    mouseMonitor->UpdateState();
    EXPECT_TRUE(mouseMonitor->GetButton(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonDown(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonUp(wb::MouseCode::Left));

    mouseMonitor->EditState(WM_LBUTTONUP, 0, 0);
    EXPECT_FALSE(mouseMonitor->GetButton(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonDown(wb::MouseCode::Left));
    EXPECT_TRUE(mouseMonitor->GetButtonUp(wb::MouseCode::Left));

    mouseMonitor->UpdateState();
    EXPECT_FALSE(mouseMonitor->GetButton(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonDown(wb::MouseCode::Left));
    EXPECT_FALSE(mouseMonitor->GetButtonUp(wb::MouseCode::Left));

    mouseMonitor->EditState(WM_LBUTTONDOWN, 0, 0);
    mouseMonitor->UpdateState();

    mouseMonitor->EditState(WM_LBUTTONUP, 0, 0);
    EXPECT_TRUE(mouseMonitor->GetButtonDoubleTap(wb::MouseCode::Left, 0.5));

    mouseMonitor->UpdateState();
}
    

