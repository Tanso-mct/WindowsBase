#include "pch.h"

#include "wb_utility/include/dx_helpers.h"
#pragma comment(lib, "wb_utility.lib")

using Microsoft::WRL::ComPtr;

#include "wb_utility_test/window_proc.h"

namespace
{
    void CreateWindowTemp(HWND& hWnd, WNDCLASSEX& wc, std::wstring className)
    {
        wc.lpszClassName = className.c_str();
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = WindowProc;

        if (!RegisterClassEx(&wc))
        {
            FAIL() << "Window classの登録に失敗しました。";
        }

        hWnd = CreateWindow
        (
            wc.lpszClassName,
            L"CreateSwapChain",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            800, 600,
            nullptr, nullptr, wc.hInstance, nullptr
        );
        if (!hWnd)
        {
            FAIL() << "Windowの作成に失敗しました。";
        }

        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }
}

TEST(DxHelpers, CreateDX12Factory)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);
    EXPECT_NE(factory.Get(), nullptr);
}

TEST(DxHelpers, CreateDX12Device)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    EXPECT_NE(device.Get(), nullptr);
    // EXPECT_EQ(featureLevel, D3D_FEATURE_LEVEL_12_2); // 環境によるため、コメントアウト
}

TEST(DxHelpers, CreateDX12CommandQueue)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandQueue> commandQueue;
    wb::CreateDX12CommandQueue(device, commandQueue);

    EXPECT_NE(commandQueue.Get(), nullptr);
}

TEST(DxHelpers, CreateSwapChain)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandQueue> commandQueue;
    wb::CreateDX12CommandQueue(device, commandQueue);

    HWND hWnd = nullptr;
    {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
        wc.lpszClassName = L"CreateSwapChain";
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = WindowProc;
        ::CreateWindowTemp(hWnd, wc, L"CreateSwapChain");
    }

    ComPtr<IDXGISwapChain3> swapChain;
    const UINT frameCount = 2;
    UINT frameIndex = 0;

    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        UINT clientWidth = rect.right - rect.left;
        UINT clientHeight = rect.bottom - rect.top;

        wb::CreateSwapChain(factory, commandQueue, frameCount, frameIndex, hWnd, clientWidth, clientHeight, swapChain);
    }

    EXPECT_NE(swapChain.Get(), nullptr);

    // Windowを破棄
    DestroyWindow(hWnd);
}

TEST(DxHelpers, ResizeSwapChain)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandQueue> commandQueue;
    wb::CreateDX12CommandQueue(device, commandQueue);

    HWND hWnd = nullptr;
    {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
        wc.lpszClassName = L"ResizeSwapChain";
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = WindowProc;
        ::CreateWindowTemp(hWnd, wc, L"ResizeSwapChain");
    }

    ComPtr<IDXGISwapChain3> swapChain;
    const UINT frameCount = 2;
    UINT frameIndex = 0;

    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        UINT clientWidth = rect.right - rect.left;
        UINT clientHeight = rect.bottom - rect.top;

        wb::CreateSwapChain(factory, commandQueue, frameCount, frameIndex, hWnd, clientWidth, clientHeight, swapChain);
    }

    EXPECT_NE(swapChain.Get(), nullptr);

    // Windowのサイズを変更
    {
        UINT newWidth = 500;
        UINT newHeight = 500;
        SetWindowPos(hWnd, nullptr, 0, 0, newWidth, newHeight, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
    }

    // SwapChainをリサイズ
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        UINT clientWidth = rect.right - rect.left;
        UINT clientHeight = rect.bottom - rect.top;

        wb::ResizeSwapChain(frameCount, frameIndex, clientWidth, clientHeight, swapChain);
    }

    // Windowを破棄
    DestroyWindow(hWnd);
}

TEST(DxHelpers, GetBuffersFromSwapChain)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandQueue> commandQueue;
    wb::CreateDX12CommandQueue(device, commandQueue);

    HWND hWnd = nullptr;
    {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
        wc.lpszClassName = L"GetBuffersFromSwapChain";
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpfnWndProc = WindowProc;
        ::CreateWindowTemp(hWnd, wc, L"GetBuffersFromSwapChain");
    }

    ComPtr<IDXGISwapChain3> swapChain;
    const UINT frameCount = 2;
    UINT frameIndex = 0;

    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        UINT clientWidth = rect.right - rect.left;
        UINT clientHeight = rect.bottom - rect.top;

        wb::CreateSwapChain(factory, commandQueue, frameCount, frameIndex, hWnd, clientWidth, clientHeight, swapChain);
    }

    EXPECT_NE(swapChain.Get(), nullptr);

    std::vector<ComPtr<ID3D12Resource>> buffers;
    wb::GetBuffersFromSwapChain(swapChain, frameCount, buffers);
    EXPECT_EQ(buffers.size(), frameCount);

    for (int i = 0; i < frameCount; ++i)
    {
        EXPECT_NE(buffers[i].Get(), nullptr);
        std::wstring bufferName = L"Buffer_" + std::to_wstring(i);
        wb::SetName(buffers[i], bufferName);
    }

    // Windowを破棄
    DestroyWindow(hWnd);
}

TEST(DxHelpers, CreateRenderTargetViewHeap)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    const UINT rtvDescriptorCount = 2; // 2つのRTVを作成
    UINT rtvDescriptorSize = 0;

    wb::CreateRenderTargetViewHeap(device, rtvDescriptorCount, rtvHeap, rtvDescriptorSize);
    EXPECT_NE(rtvHeap.Get(), nullptr);
}

TEST(DxHelpers, CreateRenderTargetView)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> texture;
    const UINT width = 256;
    const UINT height = 256;
    wb::CreateTexture2D(device, texture, width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
    EXPECT_NE(texture.Get(), nullptr);

    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    const UINT rtvDescriptorCount = 1; // 1つのRTVを作成
    UINT rtvDescriptorSize = 0;

    wb::CreateRenderTargetViewHeap(device, rtvDescriptorCount, rtvHeap, rtvDescriptorSize);
    EXPECT_NE(rtvHeap.Get(), nullptr);

    // RTVを作成
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

    device->CreateRenderTargetView(texture.Get(), &rtvDesc, rtvHeap->GetCPUDescriptorHandleForHeapStart());
}

TEST(DxHelpers, CreateCommandAllocator)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandAllocator> commandAllocator;
    wb::CreateCommandAllocator(device, commandAllocator);
    EXPECT_NE(commandAllocator.Get(), nullptr);
}

TEST(DxHelpers, CreateDepthStencil)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> depthStencil;
    const UINT clientWidth = 800;
    const UINT clientHeight = 600;

    wb::CreateDepthStencil(device, clientWidth, clientHeight, depthStencil);
    EXPECT_NE(depthStencil.Get(), nullptr);
}

TEST(DxHelpers, CreateDepthStencilViewHeap)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12DescriptorHeap> dsvHeap;
    const UINT depthStencilCount = 1; // 1つのDSVを作成

    wb::CreateDepthStencilViewHeap(device, depthStencilCount, dsvHeap);
    EXPECT_NE(dsvHeap.Get(), nullptr);
}

TEST(DxHelpers, CreateDepthStencilView)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> depthStencil;
    const UINT clientWidth = 800;
    const UINT clientHeight = 600;

    wb::CreateDepthStencil(device, clientWidth, clientHeight, depthStencil);
    EXPECT_NE(depthStencil.Get(), nullptr);

    ComPtr<ID3D12DescriptorHeap> dsvHeap;
    const UINT depthStencilCount = 1; // 1つのDSVを作成
    wb::CreateDepthStencilViewHeap(device, depthStencilCount, dsvHeap);
    EXPECT_NE(dsvHeap.Get(), nullptr);

    wb::CreateDepthStencilView(device, depthStencil, dsvHeap);
}

TEST(DxHelpers, CreateViewport)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    D3D12_VIEWPORT viewport;
    const UINT clientWidth = 800;
    const UINT clientHeight = 600;

    wb::CreateViewport(viewport, clientWidth, clientHeight);
    EXPECT_EQ(viewport.Width, static_cast<float>(clientWidth));
    EXPECT_EQ(viewport.Height, static_cast<float>(clientHeight));
}

TEST(DxHelpers, CreateScissorRect)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    D3D12_RECT scissorRect;
    const UINT clientWidth = 800;
    const UINT clientHeight = 600;

    wb::CreateScissorRect(scissorRect, clientWidth, clientHeight);
    EXPECT_EQ(scissorRect.right - scissorRect.left, static_cast<LONG>(clientWidth));
    EXPECT_EQ(scissorRect.bottom - scissorRect.top, static_cast<LONG>(clientHeight));
}

TEST(DxHelpers, CreateBuffer)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> buffer;
    const UINT size = 1024; // 1KBのバッファを作成

    wb::CreateBuffer(device, buffer, size);
    EXPECT_NE(buffer.Get(), nullptr);

    // バッファの名前を設定
    std::wstring bufferName = L"TestBuffer";
    wb::SetName(buffer, bufferName);
}

TEST(DxHelpers, UpdateBuffer)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> buffer;
    const UINT size = 1024; // 1KBのバッファを作成
    wb::CreateBuffer(device, buffer, size);

    // バッファの名前を設定
    std::wstring bufferName = L"UpdatedBuffer";
    wb::SetName(buffer, bufferName);

    // バッファに書き込むデータ
    std::vector<uint8_t> initData(size, 0xAB); // 0xABで埋める

    wb::UpdateBuffer(buffer, initData.data(), size);
}

TEST(DxHelpers, CreateTexture2D)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> texture;
    const UINT width = 256;
    const UINT height = 256;

    wb::CreateTexture2D(device, texture, width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
    EXPECT_NE(texture.Get(), nullptr);

    // テクスチャの名前を設定
    std::wstring textureName = L"TestTexture";
    wb::SetName(texture, textureName);
}

TEST(DxHelpers, CreateUploadHeap)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12Resource> uploadHeap;
    const UINT size = 1024; // 1KBのアップロードヒープを作成

    wb::CreateUploadHeap(device, uploadHeap, size);
    EXPECT_NE(uploadHeap.Get(), nullptr);

    // アップロードヒープの名前を設定
    std::wstring uploadHeapName = L"TestUploadHeap";
    wb::SetName(uploadHeap, uploadHeapName);
}

TEST(DxHelpers, AddUploadTextureToCmdList)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandAllocator> commandAllocator;
    wb::CreateCommandAllocator(device, commandAllocator);
    EXPECT_NE(commandAllocator.Get(), nullptr);

    ComPtr<ID3D12GraphicsCommandList> commandList;
    wb::CreateCommandList(device, commandAllocator, commandList);
    EXPECT_NE(commandList.Get(), nullptr);

    ComPtr<ID3D12Resource> texture;
    const UINT width = 256;
    const UINT height = 256;
    wb::CreateTexture2D(device, texture, width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
    EXPECT_NE(texture.Get(), nullptr);

    ComPtr<ID3D12Resource> uploadHeap;
    const UINT uploadHeapSize = 256 * 256 * 4; // RGBA形式のテクスチャデータサイズ
    wb::CreateUploadHeap(device, uploadHeap, uploadHeapSize);
    EXPECT_NE(uploadHeap.Get(), nullptr);

    // Commandをリセット
    wb::ResetCommand(commandList, commandAllocator);

    // テクスチャデータの初期化をするためのコマンドを追加
    std::vector<uint8_t> textureData(width * height * 4, 0xFF); // 白色のテクスチャデータ
    wb::AddUploadTextureToCmdList
    (
        commandList, texture, uploadHeap, textureData.data(), width, height, DXGI_FORMAT_R8G8B8A8_UNORM
    );
}

TEST(DxHelpers, ResetCommand)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandAllocator> commandAllocator;
    wb::CreateCommandAllocator(device, commandAllocator);
    EXPECT_NE(commandAllocator.Get(), nullptr);

    ComPtr<ID3D12GraphicsCommandList> commandList;
    wb::CreateCommandList(device, commandAllocator, commandList);
    EXPECT_NE(commandList.Get(), nullptr);

    // コマンドリストをリセット
    wb::ResetCommand(commandList, commandAllocator);
}

TEST(DxHelpers, UploadTexture)
{
    ComPtr<IDXGIFactory4> factory;
    wb::CreateDX12Factory(factory);

    ComPtr<ID3D12Device4> device;
    D3D_FEATURE_LEVEL featureLevel;
    wb::CreateDX12Device(device, featureLevel, factory);

    ComPtr<ID3D12CommandQueue> commandQueue;
    wb::CreateDX12CommandQueue(device, commandQueue);
    EXPECT_NE(commandQueue.Get(), nullptr);

    // Fenceとイベントを作成
    ComPtr<ID3D12Fence> fence;
    UINT64 fenceValue = 0;
    wb::CreateFence(device, fence, fenceValue);

    HANDLE fenceEvent = nullptr;
    wb::CreateFenceEvent(fenceEvent);

    // コマンドアロケーターとコマンドリストを作成
    ComPtr<ID3D12CommandAllocator> commandAllocator;
    wb::CreateCommandAllocator(device, commandAllocator);
    EXPECT_NE(commandAllocator.Get(), nullptr);

    ComPtr<ID3D12GraphicsCommandList> commandList;
    wb::CreateCommandList(device, commandAllocator, commandList);
    EXPECT_NE(commandList.Get(), nullptr);

    ComPtr<ID3D12Resource> texture;
    const UINT width = 256;
    const UINT height = 256;
    wb::CreateTexture2D(device, texture, width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
    EXPECT_NE(texture.Get(), nullptr);

    ComPtr<ID3D12Resource> uploadHeap;
    const UINT uploadHeapSize = width * height * 4; // RGBA形式のテクスチャデータサイズ
    wb::CreateUploadHeap(device, uploadHeap, uploadHeapSize);
    EXPECT_NE(uploadHeap.Get(), nullptr);

    // Commandをリセット
    wb::ResetCommand(commandList, commandAllocator);

    // テクスチャデータの初期化をするためのコマンドを追加
    std::unique_ptr<uint8_t[]> textureData(new uint8_t[width * height * 4]);
    wb::AddUploadTextureToCmdList
    (
        commandList, texture, uploadHeap, textureData.get(), width, height, DXGI_FORMAT_R8G8B8A8_UNORM
    );

    // コマンドリストをクローズ
    wb::CloseCommand(commandList);

    // Closeが完了するまで待つ
    wb::WaitForGPU(commandQueue, fence, fenceValue, fenceEvent);

    // コマンドを実行
    std::vector<ID3D12CommandList*> commandLists;
    commandLists.push_back(commandList.Get());
    wb::ExecuteCommand(commandQueue, commandLists.size(), commandLists);

    // GPUの完了を待つ
    wb::WaitForGPU(commandQueue, fence, fenceValue, fenceEvent);

    // フェンスイベントをクローズ
    wb::CloseFenceEvent(fenceEvent);
}