#include "pch.h"

#include "wb_render_support/include/render_target_context.h"
#pragma comment(lib, "wb_render_support.lib")

#include <memory>

TEST(RenderTargetContext, Get)
{
    const UINT renderTargetCount = 2;
    const UINT depthStencilCount = 1;

    std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
    (
        renderTargetCount, depthStencilCount
    );

    std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>>& commandAllocators 
    = renderTargetContext->CommandAllocators();

    const UINT& renderTargetCountRef = renderTargetContext->RenderTargetCount();
    EXPECT_EQ(renderTargetCountRef, renderTargetCount);

    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& renderTargets = renderTargetContext->RenderTargets();
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvDescriptorHeap = renderTargetContext->RtvDescriptorHeap();
    UINT& rtvDescriptorSize = renderTargetContext->RtvDescriptorSize();

    const UINT& depthStencilCountRef = renderTargetContext->DepthStencilCount();
    EXPECT_EQ(depthStencilCountRef, depthStencilCount);
    
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& depthStencils = renderTargetContext->DepthStencils();
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvDescriptorHeap = renderTargetContext->DsvDescriptorHeap();
    UINT& dsvDescriptorSize = renderTargetContext->DsvDescriptorSize();
}