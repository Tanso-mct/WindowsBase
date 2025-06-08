#include "pch.h"

#include "wb_monitor/include/mouse_context.h"
#pragma comment(lib, "wb_monitor.lib")

#include <memory>

TEST(MouseContext, Get)
{
    std::unique_ptr<wb::IMouseContext> mouseContext = std::make_unique<wb::MouseContext>();

    mouseContext->ButtonStates().resize(static_cast<size_t>(wb::MouseCode::Size), wb::INPUT_NONE);
    EXPECT_EQ(mouseContext->ButtonStates().size(), static_cast<size_t>(wb::MouseCode::Size));

    mouseContext->PreviousButton() = wb::MouseCode::Left;
    EXPECT_EQ(mouseContext->PreviousButton(), wb::MouseCode::Left);

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    mouseContext->PreviousButtonTime() = now;
    EXPECT_EQ(mouseContext->PreviousButtonTime(), now);

    mouseContext->ScrollDelta() = 120;
    EXPECT_EQ(mouseContext->ScrollDelta(), 120);

    mouseContext->PositionX() = 800;
    EXPECT_EQ(mouseContext->PositionX(), 800);

    mouseContext->PositionY() = 600;
    EXPECT_EQ(mouseContext->PositionY(), 600);
}