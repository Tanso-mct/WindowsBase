#include "wb_render_support/src/pch.h"
#include "wb_render_support/include/swap_chain_context.h"

wb::SwapChainContext::SwapChainContext(UINT frameCount) :
    frameCount_(frameCount),
    frameIndex_(0)
{
    
}