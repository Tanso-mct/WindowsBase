#include "pch.h"

#include "wb_render_support/include/swap_chain_context.h"
#pragma comment(lib, "wb_render_support.lib")

#include <memory>

TEST(SwapChainContext, Get)
{
    const UINT frameCount = 2;

    std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>(frameCount);

    Microsoft::WRL::ComPtr<IDXGISwapChain3>& swapChain = swapChainContext->SwapChain();
    const UINT& frameCountRef = swapChainContext->FrameCount();
    EXPECT_EQ(frameCountRef, frameCount);

    UINT& frameIndex = swapChainContext->FrameIndex();
    EXPECT_EQ(frameIndex, 0);
}