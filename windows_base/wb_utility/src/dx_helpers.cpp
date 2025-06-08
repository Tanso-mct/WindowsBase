#include "wb_utility/src/pch.h"
#include "wb_utility/include/dx_helpers.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#include "wb_utility/include/type_cast.h"

using Microsoft::WRL::ComPtr;

#include "directx12_helper/include/d3dx12.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

WB_UTILITY_API void wb::CreateDX12Factory(ComPtr<IDXGIFactory4> &factory)
{
    UINT dxgiFactoryFlags = 0;

#ifndef NDEBUG
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
        {
            debugController->EnableDebugLayer();

            // 追加のデバッグレイヤーを有効にする
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

            ComPtr<ID3D12Debug1> spDebugController1;
            debugController->QueryInterface(IID_PPV_ARGS(spDebugController1.GetAddressOf()));
            spDebugController1->SetEnableGPUBasedValidation(true);
        }
    }
#endif

    HRESULT hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(factory.GetAddressOf()));
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "DXGIファクトリの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::CreateDX12Device
(
    ComPtr<ID3D12Device4> &device, D3D_FEATURE_LEVEL &dstFeatureLevel, 
    ComPtr<IDXGIFactory4> &factory
){
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_2,
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    ComPtr<IDXGIAdapter1> adapter;
    ComPtr<IDXGIFactory6> factory6;
    HRESULT hr = E_FAIL;

    if (SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(factory6.GetAddressOf()))))
    {
        // ファクトリ6と互換性があるので、ハイスペックなアダプターを探す
        for (
            UINT adapterIndex = 0;
            SUCCEEDED(factory6->EnumAdapterByGpuPreference
            ( // GPU機能ライド用アダプターをファクトリから1つずつ入手
                adapterIndex,
                DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                IID_PPV_ARGS(adapter.GetAddressOf())
            ));
            ++adapterIndex
        ){
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

            hr = E_FAIL;

            for (UINT featureLevel = 0; featureLevel < numFeatureLevels; featureLevel++)
            {
                dstFeatureLevel = featureLevels[featureLevel];
                if (SUCCEEDED(hr = D3D12CreateDevice(adapter.Get(), dstFeatureLevel, _uuidof(ID3D12Device), &device)))
                {
                    break;
                }
            }

            if (SUCCEEDED(hr))
                break;
        }
    }

    if (device.Get() == nullptr)
    {
        // すべてのアダプターで使えるものを探す
        for (UINT adapterIndex = 0; SUCCEEDED(factory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

            hr = E_FAIL;

            for (UINT featureLevel = 0; featureLevel < numFeatureLevels; featureLevel++)
            {
                dstFeatureLevel = featureLevels[featureLevel];
                if (SUCCEEDED(hr = D3D12CreateDevice(adapter.Get(), dstFeatureLevel, _uuidof(ID3D12Device), &device)))
                {
                    break;
                }

            }

            if (SUCCEEDED(hr))
                break;
        }
    }

    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "D3D12デバイスの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::CreateDX12CommandQueue
(
    const ComPtr<ID3D12Device4> &device, ComPtr<ID3D12CommandQueue> &commandQueue
){
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()));

    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "D3D12コマンドキューの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::SetName(const ComPtr<ID3D12Resource> &object, const std::wstring &name)
{
    HRESULT hr = object->SetName(name.c_str());
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "SetName : オブジェクトの名前設定に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::CreateSwapChain
(
    const Microsoft::WRL::ComPtr<IDXGIFactory4> &factory, 
    const Microsoft::WRL::ComPtr<ID3D12CommandQueue> &commandQueue,
    const UINT &frameCount, UINT &frameIndex,
    HWND hWnd, const UINT &clientWidth, const UINT &clientHeight,
    Microsoft::WRL::ComPtr<IDXGISwapChain3> &swapChain
){
    HRESULT hr = E_FAIL;

    DXGI_SWAP_CHAIN_DESC1 desc = {};
    desc.BufferCount = frameCount;
    desc.Width = clientWidth;
    desc.Height = clientHeight;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain1;

    hr = factory->CreateSwapChainForHwnd
    (
        commandQueue.Get(),
        hWnd,
        &desc,
        nullptr,
        nullptr,
        &swapChain1
    );
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateSwapChain : SwapChainの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    hr = swapChain1.As(&(swapChain));
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateSwapChain : IDXGISwapChain3への変換に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    // 現在のバックバッファインデックスを取得
    frameIndex = swapChain->GetCurrentBackBufferIndex();
}

WB_UTILITY_API void wb::ResizeSwapChain
(
    const UINT &frameCount, UINT &frameIndex, 
    const UINT &clientWidth, const UINT &clientHeight, 
    const ComPtr<IDXGISwapChain3> &swapChain
){
    HRESULT hr = swapChain->ResizeBuffers
    (
        frameCount, clientWidth, clientHeight,
        DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    );
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "ResizeSwapChain : SwapChainのリサイズに失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    // リサイズ後のバックバッファインデックスを更新
    frameIndex = swapChain->GetCurrentBackBufferIndex();
}

WB_UTILITY_API void wb::GetBuffersFromSwapChain
(
    const ComPtr<IDXGISwapChain3> &swapChain, const UINT &frameCount,
    std::vector<ComPtr<ID3D12Resource>> &buffers
){
    HRESULT hr = E_FAIL;

    buffers.clear();
    for (UINT i = 0; i < frameCount; ++i)
    {
        ComPtr<ID3D12Resource> buffer;
        hr = swapChain->GetBuffer(i, IID_PPV_ARGS(buffer.GetAddressOf()));
        if (FAILED(hr))
        {
            std::string hrStr;
            wb::SpecifiedAs(hr, hrStr);

            std::string err = wb::ConsoleLogErr
            (
                __FILE__, __LINE__, __FUNCTION__,
                {
                    "GetBuffersFromSwapChain : バッファの取得に失敗しました",
                    hrStr.c_str()
                }
            );
            wb::ErrorNotify("DX_HELPERS", err.c_str());
            wb::QuitProgram();
        }

        buffers.push_back(buffer);
    }
}

WB_UTILITY_API void wb::CreateRenderTargetViewHeap
(
    const ComPtr<ID3D12Device4> &device, 
    const UINT &descriptorCount, ComPtr<ID3D12DescriptorHeap> &rtvHeap, UINT &rtvDescriptorSize
){
    HRESULT hr = E_FAIL;

    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.NumDescriptors = descriptorCount;
    heapDesc.NodeMask = 0;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    
    hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(rtvHeap.GetAddressOf()));
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateRenderTargetViewHeap : RTVヒープの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    // RTVのハンドルサイズを取得
    rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

WB_UTILITY_API void wb::CreateRenderTargetView
(
    const ComPtr<ID3D12Device4> &device, 
    const UINT& descriptorCount, std::vector<ComPtr<ID3D12Resource>> &renderTargets,
    const ComPtr<ID3D12DescriptorHeap> &rtvHeap, UINT rtvDescriptorSize
){
    HRESULT hr = E_FAIL;

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());

    for (UINT i = 0; i < descriptorCount; i++)
    {
        device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, rtvDescriptorSize);
    }
}

WB_UTILITY_API void wb::CreateCommandAllocator
(
    const ComPtr<ID3D12Device4> &device, 
    ComPtr<ID3D12CommandAllocator> &commandAllocators
){
    HRESULT hr = E_FAIL;
    
    hr = device->CreateCommandAllocator
    (
        D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocators.GetAddressOf())
    );
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateCommandAllocator : コマンドアロケーターの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::CreateDepthStencil
(
    const ComPtr<ID3D12Device4> &device, 
    const UINT &clientWidth, const UINT &clientHeight, ComPtr<ID3D12Resource> &depthStencil
){
    HRESULT hr = E_FAIL;

    D3D12_CLEAR_VALUE clearValue = {};
    clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    clearValue.DepthStencil.Depth = 1.0f;
    clearValue.DepthStencil.Stencil = 0;

    const CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
    const CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D
    (
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        clientWidth, clientHeight,
        1, 0, 1, 0,
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
    );

    hr = device->CreateCommittedResource
    (
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clearValue,
        IID_PPV_ARGS(depthStencil.GetAddressOf())
    );
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateDepthStencil : 深度ステンシルバッファの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::CreateDepthStencilViewHeap
(
    const ComPtr<ID3D12Device4> &device, 
    const UINT &depthStencilCount, ComPtr<ID3D12DescriptorHeap> &dsvHeap
){
    HRESULT hr = E_FAIL;

    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.NumDescriptors = depthStencilCount;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(dsvHeap.GetAddressOf()));
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateDepthStencilViewHeap : DSVヒープの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::CreateDepthStencilView
(
    const ComPtr<ID3D12Device4> &device, 
    const ComPtr<ID3D12Resource> &depthStencil, ComPtr<ID3D12DescriptorHeap> dsvHeap
){
    D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    desc.Flags = D3D12_DSV_FLAG_NONE;

    device->CreateDepthStencilView
    (
        depthStencil.Get(), &desc, 
        dsvHeap->GetCPUDescriptorHandleForHeapStart()
    );
}

WB_UTILITY_API void wb::CreateViewport(D3D12_VIEWPORT &viewport, const UINT &clientWidth, const UINT &clientHeight)
{
    viewport.Width = static_cast<float>(clientWidth);
    viewport.Height = static_cast<float>(clientHeight);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
}

WB_UTILITY_API void wb::CreateScissorRect(D3D12_RECT &scissorRect, const UINT &clientWidth, const UINT &clientHeight)
{
    scissorRect.top = 0;
    scissorRect.bottom = clientHeight;
    scissorRect.left = 0;
    scissorRect.right = clientWidth;
}

WB_UTILITY_API void wb::CreateBuffer
(
    const ComPtr<ID3D12Device4> &device, 
    ComPtr<ID3D12Resource> &buffer,  const UINT& size
){
    HRESULT hr = E_FAIL;

    CD3DX12_HEAP_PROPERTIES upheapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

    UINT fixedSize = (size + 255) & ~255;
    CD3DX12_RESOURCE_DESC sourceDesc = CD3DX12_RESOURCE_DESC::Buffer(fixedSize);

    hr = device->CreateCommittedResource
    (
        &upheapProp,   //CD3DX12_HEAP_PROPERTIESはD3D12のMSサンプルが用意しているラッパー構造体
        D3D12_HEAP_FLAG_NONE,
        &sourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(buffer.GetAddressOf())
    );

    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateBuffer : バッファの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::UpdateBuffer
(
    const ComPtr<ID3D12Resource> &buffer, 
    const void *initData, const UINT& size
){
    HRESULT hr = E_FAIL;

    uint8_t* memory = nullptr;

    // バッファのマッピング
    hr = buffer->Map(0, nullptr, reinterpret_cast<void**>(&memory));
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateBuffer : バッファのマッピングに失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    // 初期化データをバッファにコピー
    std::memcpy(memory, initData, size);

    // マッピングを解除
    buffer->Unmap(0, nullptr);
}

WB_UTILITY_API void wb::CreateTexture2D
(
    const ComPtr<ID3D12Device4> &device, 
    ComPtr<ID3D12Resource> &texture, 
    const UINT &width, const UINT &height, const DXGI_FORMAT &format
){
    HRESULT hr = E_FAIL;

    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D
    (
        format, width, height, 1, 0, 1, 0,
        D3D12_RESOURCE_FLAG_NONE, D3D12_TEXTURE_LAYOUT_UNKNOWN
    );
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);

    hr = device->CreateCommittedResource
    (
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(texture.GetAddressOf())
    );
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateTexture2D : テクスチャの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::CreateUploadHeap
(
    const ComPtr<ID3D12Device4> &device, 
    ComPtr<ID3D12Resource> &uploadHeap, const UINT &size
){
    CD3DX12_HEAP_PROPERTIES upHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

    device->CreateCommittedResource
    (
        &upHeap,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(uploadHeap.GetAddressOf())
    );
}

WB_UTILITY_API UINT wb::GetDXGIFormatByteSize(const DXGI_FORMAT &format)
{
    switch (format)
    {
    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
        return 4; // 32 bits = 4 bytes

    default:
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"GetDXGIFormatByteSize : 未対応のフォーマットです"}
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();

        return 0; // 未対応のフォーマット
    }
}

WB_UTILITY_API void wb::AddUploadTextureToCmdList
(
    const ComPtr<ID3D12GraphicsCommandList> &cmdList, 
    const ComPtr<ID3D12Resource> &texture, const ComPtr<ID3D12Resource> &uploadHeap, 
    const void* data, const UINT &width, const UINT &height, const DXGI_FORMAT &format
){
    // テクスチャのサブリソースデータを更新するためのサブリソースデータ構造体を作成
    const UINT subresourceCount = 1;
    D3D12_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pData = data;
    subresourceData.RowPitch = width * wb::GetDXGIFormatByteSize(format);
    subresourceData.SlicePitch = subresourceData.RowPitch * height;

    // コマンドリストにテクスチャの更新コマンドを追加
    // UploadHeapを使用して、テクスチャのデータをGPUに転送する
    // データの流れとしては、pData->UploadHeap->Texture
    UpdateSubresources
    (
        cmdList.Get(), texture.Get(), uploadHeap.Get(), 0, 0, subresourceCount, &subresourceData
    );

    // テクスチャの状態を更新するためのリソースバリアを追加
    CD3DX12_RESOURCE_BARRIER tra = CD3DX12_RESOURCE_BARRIER::Transition
    (
        texture.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
    cmdList->ResourceBarrier(1, &tra);

    // UploadHeapの削除命令を追加
    cmdList->DiscardResource(uploadHeap.Get(), nullptr );
}

WB_UTILITY_API void wb::CreateCommandList
(
    const ComPtr<ID3D12Device4> &device, 
    const ComPtr<ID3D12CommandAllocator> &commandAllocator, 
    ComPtr<ID3D12GraphicsCommandList> &commandList
){
    HRESULT hr = E_FAIL;

    // コマンドリストの作成
    hr = device->CreateCommandList
    (
        0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr,
        IID_PPV_ARGS(commandList.GetAddressOf())
    );
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateCommandList : コマンドリストの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    // コマンドリストを初期化状態にする
    commandList->Close();
}

WB_UTILITY_API void wb::CreateFence
(
    const ComPtr<ID3D12Device4> &device, 
    ComPtr<ID3D12Fence> &fence, const UINT64 &initialValue
){
    HRESULT hr = E_FAIL;

    // フェンスの作成
    hr = device->CreateFence
    (
        initialValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf())
    );
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CreateFence : フェンスの作成に失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::CreateFenceEvent(HANDLE &fenceEvent)
{
    // フェンスイベントの作成
    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (fenceEvent == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"CreateFenceEvent : フェンスイベントの作成に失敗しました"}
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::CloseFenceEvent(HANDLE &fenceEvent)
{
    if (fenceEvent != nullptr)
    {
        bool result = CloseHandle(fenceEvent);
        if (!result)
        {
            std::string err = wb::ConsoleLogErr
            (
                __FILE__, __LINE__, __FUNCTION__,
                {"CloseFenceEvent : フェンスイベントのクローズに失敗しました"}
            );
            wb::ErrorNotify("DX_HELPERS", err.c_str());
            wb::QuitProgram();
        }

        fenceEvent = nullptr;
    }
}

WB_UTILITY_API void wb::CloseCommand(const ComPtr<ID3D12GraphicsCommandList> &commandList)
{
    HRESULT hr = commandList->Close();
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "CloseCommandList : コマンドリストのクローズに失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::ResetCommand
(
    const ComPtr<ID3D12GraphicsCommandList> &commandList, 
    const ComPtr<ID3D12CommandAllocator> &commandAllocator
){
    HRESULT hr = E_FAIL;

    hr = commandAllocator->Reset();
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "ResetCommandList : コマンドアロケーターのリセットに失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    hr = commandList->Reset(commandAllocator.Get(), nullptr);
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "ResetCommandList : コマンドリストのリセットに失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
}

WB_UTILITY_API void wb::ExecuteCommand
(
    const ComPtr<ID3D12CommandQueue> &commandQueue, 
    const UINT &commandListCount, std::vector<ID3D12CommandList*>& commandLists
){
    commandQueue->ExecuteCommandLists
    (
        commandListCount, commandLists.data()
    );
}

WB_UTILITY_API void wb::WaitForGPU
(
    const ComPtr<ID3D12CommandQueue> &commandQueue, 
    const ComPtr<ID3D12Fence> &fence, UINT64 &fenceValue, HANDLE& fenceEvent
){
    HRESULT hr = E_FAIL;

    // コマンドキューにフェンスの検知シグナルを追加
    hr = commandQueue->Signal(fence.Get(), fenceValue);
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "WaitForGPU : コマンドキューのシグナルに失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    // フェンス処理終了確認イベントをセット
    hr = fence->SetEventOnCompletion(fenceValue, fenceEvent);
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "WaitForGPU : フェンスのイベントセットに失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    // イベント実行待ち 時間制限なし
    WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);

    // 終了したので該当フレームのフェンスカウント加算
    fenceValue++;
}

WB_UTILITY_API void wb::WaitFrameForGPU
(
    const Microsoft::WRL::ComPtr<ID3D12CommandQueue> &commandQueue, 
    const Microsoft::WRL::ComPtr<ID3D12Fence> &fence, HANDLE &fenceEvent, 
    const UINT64 &signalFenceValue, UINT64 &waitFenceValue
){
    HRESULT hr = E_FAIL;

    // コマンドキューにフェンスの検知シグナルを追加
    hr = commandQueue->Signal(fence.Get(), signalFenceValue);
    if (FAILED(hr))
    {
        std::string hrStr;
        wb::SpecifiedAs(hr, hrStr);

        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "WaitForGPU : コマンドキューのシグナルに失敗しました",
                hrStr.c_str()
            }
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    if (fence->GetCompletedValue() < waitFenceValue)
    {
        // フェンスの完了値が待機する値より小さい場合、イベントをセット
        hr = fence->SetEventOnCompletion(waitFenceValue, fenceEvent);
        if (FAILED(hr))
        {
            std::string hrStr;
            wb::SpecifiedAs(hr, hrStr);

            std::string err = wb::ConsoleLogErr
            (
                __FILE__, __LINE__, __FUNCTION__,
                {
                    "WaitForGPU : フェンスのイベントセットに失敗しました",
                    hrStr.c_str()
                }
            );
            wb::ErrorNotify("DX_HELPERS", err.c_str());
            wb::QuitProgram();
        }

        // イベント実行待ち 時間制限なし
        WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);
    }

    // フェンスの完了値を更新
    waitFenceValue = signalFenceValue + 1;
}

WB_UTILITY_API void wb::SetBarrier
(
    const ComPtr<ID3D12GraphicsCommandList> &cmdList, 
    const ComPtr<ID3D12Resource> &resource, const D3D12_RESOURCE_STATES &beforeState, 
    const D3D12_RESOURCE_STATES &afterState
){
    // リソースの状態を変更するためのバリアを設定
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition
    (
        resource.Get(), beforeState, afterState
    );
    
    cmdList->ResourceBarrier(1, &barrier);
}

WB_UTILITY_API void wb::SetRenderTargets
(
    const ComPtr<ID3D12GraphicsCommandList> &cmdList, 
    const ComPtr<ID3D12DescriptorHeap> &rtvHeap, const UINT &rtvIndex, const UINT &rtvDescriptorSize, 
    const ComPtr<ID3D12DescriptorHeap> &dsvHeap, const UINT &dsvIndex, const UINT &dsvDescriptorSize
){
    if (!rtvHeap || !dsvHeap)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"SetRenderTargets : RTVまたはDSVヒープが無効です"}
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), rtvIndex, rtvDescriptorSize);
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(dsvHeap->GetCPUDescriptorHandleForHeapStart(), dsvIndex, dsvDescriptorSize);

    // レンダーターゲットビューと深度ステンシルビューを設定
    cmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
}

WB_UTILITY_API void wb::SetRenderTargets
(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList, 
    const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &rtvHeap, const UINT &rtvIndex, const UINT &rtvDescriptorSize
){
    if (!rtvHeap)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"SetRenderTargets : RTVヒープが無効です"}
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), rtvIndex, rtvDescriptorSize);

    // レンダーターゲットビューを設定
    cmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
}

WB_UTILITY_API void wb::ClearRenderTargetViews
(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList, 
    const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &rtvHeap, 
    const UINT &rtvIndex, const UINT &rtvDescriptorSize, 
    const FLOAT (&clearColor)[4]
){
    if (!rtvHeap)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ClearRenderTargetViews : RTVヒープが無効です"}
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }
    
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), rtvIndex, rtvDescriptorSize);

    // レンダーターゲットビューをクリア
    cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

WB_UTILITY_API void wb::ClearDepthStencilView
(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList, 
    const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> &dsvHeap, const UINT &dsvIndex, const UINT &dsvDescriptorSize, 
    const D3D12_RECT &scissorRect, const float &depth, const UINT8 &stencil
){
    if (!dsvHeap)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ClearDepthStencilView : DSVヒープが無効です"}
        );
        wb::ErrorNotify("DX_HELPERS", err.c_str());
        wb::QuitProgram();
    }

    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(dsvHeap->GetCPUDescriptorHandleForHeapStart(), dsvIndex, dsvDescriptorSize);

    // 深度ステンシルビューをクリア
    cmdList->ClearDepthStencilView
    (
        dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
        depth, stencil, 1, &scissorRect
    );
}