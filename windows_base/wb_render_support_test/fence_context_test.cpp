#include "pch.h"

#include "wb_render_support/include/fence_context.h"
#pragma comment(lib, "wb_render_support.lib")

TEST(FenceContext, CreateAndDestroy)
{
    const UINT frameCount = 2;
    std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>(frameCount);
    
    EXPECT_EQ(fenceContext->Fence().Get(), nullptr);
    EXPECT_EQ(fenceContext->FenceValues().size(), frameCount);
    EXPECT_EQ(fenceContext->FenceEvent(), nullptr);

    fenceContext.reset(); // 明示的に解放
}