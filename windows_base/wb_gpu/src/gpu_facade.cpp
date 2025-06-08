#include "wb_gpu/src/pch.h"
#include "wb_gpu/include/gpu_facade.h"

#include "wb_utility/include/dx_helpers.h"
#pragma comment(lib, "wb_utility.lib")

#include "wb_gpu/include/gpu_context.h"

wb::GpuFacade::GpuFacade()
{
    gpuContext_ = std::make_unique<wb::GpuContext>();
}

void wb::GpuFacade::Create()
{
    wb::CreateDX12Factory(gpuContext_->DXGIFactory());
    wb::CreateDX12Device(gpuContext_->D3D12Device(), gpuContext_->FeatureLevel(), gpuContext_->DXGIFactory());
    wb::CreateDX12CommandQueue(gpuContext_->D3D12Device(), gpuContext_->CommandQueue());

    gpuContext_->IsCreated() = true;
}