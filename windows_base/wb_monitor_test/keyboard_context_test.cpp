#include "pch.h"

#include "wb_monitor/include/keyboard_context.h"
#pragma comment(lib, "wb_monitor.lib")

#include <memory>

TEST(KeyboardContext, Get)
{
    std::unique_ptr<wb::IKeyboardContext> context = std::make_unique<wb::KeyboardContext>();

    context->KeyStates().resize(static_cast<size_t>(wb::KeyCode::Size), 0);
    EXPECT_EQ(context->KeyStates().size(), static_cast<size_t>(wb::KeyCode::Size));

    context->PreviousKey() = wb::KeyCode::A;
    EXPECT_EQ(context->PreviousKey(), wb::KeyCode::A);

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    context->PreviousKeyTime() = now;
    EXPECT_EQ(context->PreviousKeyTime(), now);
}