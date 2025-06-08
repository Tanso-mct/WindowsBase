#include "wb_render_support/src/pch.h"
#include "wb_render_support/include/render_target_context.h"

wb::RenderTargetContext::RenderTargetContext(UINT renderTargetCount, UINT depthStencilCount) : 
    renderTargetCount_(renderTargetCount),
    depthStencilCount_(depthStencilCount),
    rtvDescriptorSize_(0),
    dsvDescriptorSize_(0)
{
    renderTargets_.resize(renderTargetCount_);
    commandAllocators_.resize(renderTargetCount_);
    depthStencils_.resize(depthStencilCount_);
}