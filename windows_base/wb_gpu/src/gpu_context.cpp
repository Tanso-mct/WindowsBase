#include "wb_gpu/src/pch.h"
#include "wb_gpu/include/gpu_context.h"

wb::GpuContext &wb::GpuContext::Instance()
{
    static wb::GpuContext instance = wb::GpuContext();
    return instance;
}

bool &wb::GpuContext::IsCreated()
{
    wb::GpuContext &instance = Instance();
    return instance.isCreated_;
}

Microsoft::WRL::ComPtr<IDXGIFactory4> &wb::GpuContext::DXGIFactory()
{
    wb::GpuContext &instance = Instance();
    return instance.dxgiFactory_;
}

Microsoft::WRL::ComPtr<ID3D12Device4> &wb::GpuContext::D3D12Device()
{
    wb::GpuContext &instance = Instance();
    return instance.d3d12Device_;
}

D3D_FEATURE_LEVEL &wb::GpuContext::FeatureLevel()
{
    wb::GpuContext &instance = Instance();
    return instance.featureLevel_;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> &wb::GpuContext::CommandQueue()
{
    wb::GpuContext &instance = Instance();
    return instance.commandQueue_;
}