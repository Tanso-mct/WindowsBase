#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <memory>

namespace wb
{
    class IGpuContext
    {
    public:
        virtual ~IGpuContext() = default;

        virtual bool &IsCreated() = 0;

        virtual Microsoft::WRL::ComPtr<IDXGIFactory4> &DXGIFactory() = 0;
        virtual Microsoft::WRL::ComPtr<ID3D12Device4> &D3D12Device() = 0;
        virtual D3D_FEATURE_LEVEL &FeatureLevel() = 0;
        virtual Microsoft::WRL::ComPtr<ID3D12CommandQueue> &CommandQueue() = 0;
    };

    class IGpuFacade
    {
    public:
        virtual ~IGpuFacade() = default;

        virtual bool IsCreated() = 0;
        virtual void Create() = 0;
        virtual std::unique_ptr<IGpuContext> &GpuContext() = 0;
    };

} // namespace wb