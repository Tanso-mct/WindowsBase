#include "wb_render_support/src/pch.h"
#include "wb_render_support/include/fence_context.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#include "wb_utility/include/dx_helpers.h"
#pragma comment(lib, "wb_utility.lib")

wb::FenceContext::FenceContext(UINT frameCount)
{
    fenceValues_.resize(frameCount, DEFAULT_FENCE_VALUE);
}

wb::FenceContext::~FenceContext()
{
    wb::CloseFenceEvent(fenceEvent_);
}