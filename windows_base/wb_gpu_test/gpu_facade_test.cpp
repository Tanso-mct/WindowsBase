#include "pch.h"

#include "wb_gpu/include/gpu_facade.h"
#pragma comment(lib, "wb_gpu.lib")

TEST(GpuFacade, Create)
{
    std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
    
    EXPECT_FALSE(gpuFacade->IsCreated());
    gpuFacade->Create();
    EXPECT_TRUE(gpuFacade->IsCreated());
}