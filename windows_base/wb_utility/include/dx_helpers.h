#pragma once

#include "wb_utility/include/dll_config.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <string>
#include <vector>

namespace wb
{
    /*******************************************************************************************************************
     * GPU関連インスタンスの生成
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateDX12Factory(Microsoft::WRL::ComPtr<IDXGIFactory4> &factory);

    WB_UTILITY_API void CreateDX12Device
    (
        Microsoft::WRL::ComPtr<ID3D12Device4> &device, D3D_FEATURE_LEVEL& dstFeatureLevel,
        Microsoft::WRL::ComPtr<IDXGIFactory4> &factory
    );

    WB_UTILITY_API void CreateDX12CommandQueue
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, Microsoft::WRL::ComPtr<ID3D12CommandQueue> &commandQueue
    );

    /*******************************************************************************************************************
     * ID3D12Object関連
    /******************************************************************************************************************/

    WB_UTILITY_API void SetName(const Microsoft::WRL::ComPtr<ID3D12Resource> &object, const std::wstring& name);

    /*******************************************************************************************************************
     * SwapChain関連
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateSwapChain
    (
        const Microsoft::WRL::ComPtr<IDXGIFactory4> &factory, 
        const Microsoft::WRL::ComPtr<ID3D12CommandQueue> &commandQueue,
        const UINT &frameCount, UINT &frameIndex,
        HWND hWnd, const UINT &clientWidth, const UINT &clientHeight,
        Microsoft::WRL::ComPtr<IDXGISwapChain3> &swapChain
    );

    WB_UTILITY_API void ResizeSwapChain
    (
        const UINT &frameCount, UINT &frameIndex,
        const UINT &clientWidth, const UINT &clientHeight,
        const Microsoft::WRL::ComPtr<IDXGISwapChain3> &swapChain
    );

    WB_UTILITY_API void GetBuffersFromSwapChain
    (
        const Microsoft::WRL::ComPtr<IDXGISwapChain3> &swapChain, const UINT& frameCount,
        std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> &buffers
    );

    /*******************************************************************************************************************
     * RenderTarget関連
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateRenderTargetViewHeap
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        const UINT& descriptorCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& rtvHeap, UINT& rtvDescriptorSize
    );

    WB_UTILITY_API void CreateRenderTargetView
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        const UINT& descriptorCount, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> &renderTargets,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &rtvHeap, UINT rtvDescriptorSize
    );

    /*******************************************************************************************************************
     * CommandAllocator関連
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateCommandAllocator
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> &commandAllocator
    );

    /*******************************************************************************************************************
     * DepthStencil関連
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateDepthStencil
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        const UINT& clientWidth, const UINT& clientHeight, Microsoft::WRL::ComPtr<ID3D12Resource>& depthStencil
    );

    WB_UTILITY_API void CreateDepthStencilViewHeap
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        const UINT& depthStencilCount, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& dsvHeap
    );

    WB_UTILITY_API void CreateDepthStencilView
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device,
        const Microsoft::WRL::ComPtr<ID3D12Resource> &depthStencil,
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap
    );

    /*******************************************************************************************************************
     * ViewportとScissorRectの作成
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateViewport
    (
        D3D12_VIEWPORT& viewport, const UINT& clientWidth, const UINT& clientHeight
    );

    WB_UTILITY_API void CreateScissorRect
    (
        D3D12_RECT& scissorRect, const UINT& clientWidth, const UINT& clientHeight
    );

    /*******************************************************************************************************************
     * Buffer関連
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateBuffer
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        Microsoft::WRL::ComPtr<ID3D12Resource>& buffer, const UINT& size
    );

    WB_UTILITY_API void UpdateBuffer
    (
        const Microsoft::WRL::ComPtr<ID3D12Resource>& buffer,
        const void* initData, const UINT& size
    );

    /*******************************************************************************************************************
     * Texture関連
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateTexture2D
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device,
        Microsoft::WRL::ComPtr<ID3D12Resource>& texture, 
        const UINT& width, const UINT& height, const DXGI_FORMAT& format
    );

    WB_UTILITY_API void CreateUploadHeap
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device,
        Microsoft::WRL::ComPtr<ID3D12Resource>& uploadHeap, const UINT& size
    );

    WB_UTILITY_API UINT GetDXGIFormatByteSize(const DXGI_FORMAT& format);

    WB_UTILITY_API void AddUploadTextureToCmdList
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList,
        const Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const Microsoft::WRL::ComPtr<ID3D12Resource>& uploadHeap, 
        const void* data, const UINT& width, const UINT& height, const DXGI_FORMAT& format
    );

    /*******************************************************************************************************************
     * CommandList関連
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateCommandList
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device,
        const Microsoft::WRL::ComPtr<ID3D12CommandAllocator> &commandAllocator,
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &commandList
    );

    /*******************************************************************************************************************
     * Fence関連
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateFence
    (
        const Microsoft::WRL::ComPtr<ID3D12Device4> &device, 
        Microsoft::WRL::ComPtr<ID3D12Fence> &fence, const UINT64 &initialValue = 0
    );

    /*******************************************************************************************************************
     * Fenceイベント関連
    /******************************************************************************************************************/

    WB_UTILITY_API void CreateFenceEvent(HANDLE& fenceEvent);

    WB_UTILITY_API void CloseFenceEvent(HANDLE& fenceEvent);

    /*******************************************************************************************************************
     * Commandを実行することに関連するものら
    /******************************************************************************************************************/

    WB_UTILITY_API void CloseCommand
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &commandList
    );

    WB_UTILITY_API void ResetCommand
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
        const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& commandAllocator
    );

    WB_UTILITY_API void ExecuteCommand
    (
        const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue,
        const UINT& commandListCount, std::vector<ID3D12CommandList*>& commandLists
    );

    WB_UTILITY_API void WaitForGPU
    (
        const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue,
        const Microsoft::WRL::ComPtr<ID3D12Fence>& fence, UINT64& fenceValue, HANDLE& fenceEvent
    );

    WB_UTILITY_API void WaitFrameForGPU
    (
        const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue,
        const Microsoft::WRL::ComPtr<ID3D12Fence>& fence, HANDLE& fenceEvent,
        const UINT64& signalFenceValue, UINT64 &waitFenceValue
    );

    WB_UTILITY_API void SetBarrier
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
        const Microsoft::WRL::ComPtr<ID3D12Resource> &resource,
        const D3D12_RESOURCE_STATES &beforeState, const D3D12_RESOURCE_STATES &afterState
    );

    WB_UTILITY_API void SetRenderTargets
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &rtvHeap,
        const UINT &rtvIndex, const UINT &rtvDescriptorSize,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &dsvHeap,
        const UINT &dsvIndex, const UINT &dsvDescriptorSize
    );

    WB_UTILITY_API void SetRenderTargets
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &rtvHeap,
        const UINT &rtvIndex, const UINT &rtvDescriptorSize
    );

    WB_UTILITY_API void ClearRenderTargetViews
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &rtvHeap,
        const UINT &rtvIndex, const UINT &rtvDescriptorSize,
        const FLOAT (&clearColor)[4]
    );

    WB_UTILITY_API void ClearDepthStencilView
    (
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &dsvHeap,
        const UINT &dsvIndex, const UINT &dsvDescriptorSize,
        const D3D12_RECT &scissorRect, const float &depth = 1.0f, const UINT8 &stencil = 0
    );

} // namespace wb