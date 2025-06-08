#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <vector>

namespace wb
{
    class IRenderTargetContext
    {
    public:
        virtual ~IRenderTargetContext() = default;

        virtual std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> &CommandAllocators() = 0;

        virtual const UINT &RenderTargetCount() const = 0;
        virtual std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> &RenderTargets() = 0;
        virtual Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &RtvDescriptorHeap() = 0;
        virtual UINT &RtvDescriptorSize() = 0;

        virtual const UINT &DepthStencilCount() const = 0;
        virtual std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> &DepthStencils() = 0;
        virtual Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &DsvDescriptorHeap() = 0;
        virtual UINT &DsvDescriptorSize() = 0;

        virtual D3D12_VIEWPORT& ViewPort() = 0;
        virtual D3D12_RECT& ScissorRect() = 0;
    };

    class ISwapChainContext
    {
    public:
        virtual ~ISwapChainContext() = default;

        virtual Microsoft::WRL::ComPtr<IDXGISwapChain3> &SwapChain() = 0;
        virtual const UINT& FrameCount() const = 0;
        virtual UINT &FrameIndex() = 0;
    };

    class IFenceContext
    {
    public:
        virtual ~IFenceContext() = default;

        virtual Microsoft::WRL::ComPtr<ID3D12Fence> &Fence() = 0;
        virtual std::vector<UINT64> &FenceValues() = 0;
        virtual HANDLE &FenceEvent() = 0;
    };

} // namespace wb
