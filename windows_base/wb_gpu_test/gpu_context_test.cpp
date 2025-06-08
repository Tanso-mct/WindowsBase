#include "pch.h"

#include "wb_gpu/include/gpu_context.h"
#pragma comment(lib, "wb_gpu.lib")

#include <memory>

TEST(GpuContext, Get)
{
    std::unique_ptr<wb::IGpuContext> gpuContext = std::make_unique<wb::GpuContext>();

    EXPECT_EQ(gpuContext->DXGIFactory().Get(), nullptr);
    EXPECT_EQ(gpuContext->D3D12Device().Get(), nullptr);
    EXPECT_EQ(gpuContext->FeatureLevel(), D3D_FEATURE_LEVEL_12_0);
    EXPECT_EQ(gpuContext->CommandQueue().Get(), nullptr);
}