#include "pch.h"

#include "wb_window/include/window_context.h"
#include "wb_window/include/window_facade.h"
#pragma comment(lib, "wb_window.lib")

#include "wb_gpu/include/gpu_facade.h"
#pragma comment(lib, "wb_gpu.lib")

#include "wb_render_support/include/swap_chain_context.h"
#include "wb_render_support/include/render_target_context.h"
#include "wb_render_support/include/fence_context.h"
#pragma comment(lib, "wb_render_support.lib")

#include "wb_utility/include/dx_helpers.h"
#pragma comment(lib, "wb_utility.lib")

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "wb_window_test/window_proc.h"

namespace window_facade_test
{

    constexpr UINT RENDER_TARGET_COUNT = 2;
    constexpr UINT DEPTH_STENCIL_COUNT = 1;

}

TEST(WindowFacade, Initialize)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"Initialize";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());
}

TEST(WindowFacade, CreateAndDestroy)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"CreateAndDestroy";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());

    {
        std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated())
        {
            gpuFacade->Create();
        }
    }

    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = windowFacade->GetName().data();

        windowFacade->Create(wc);
    }
    EXPECT_TRUE(windowFacade->IsCreated());

    // ウィンドウの破棄
    windowFacade->Destroy();
    windowFacade->Destroyed();
}

TEST(WindowFacade, Resize)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"Resize";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());

    {
        std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated())
        {
            gpuFacade->Create();
        }
    }

    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = windowFacade->GetName().data();

        windowFacade->Create(wc);
    }
    EXPECT_TRUE(windowFacade->IsCreated());

    // ウィンドウのサイズを変更
    windowFacade->Resize(200, 200);

    // サイズ変更による処理を実行する。本来はウィンドウのメッセージループ内で呼ばれるべきだが、テストでは直接呼び出す
    windowFacade->Resized();

    // ウィンドウの破棄
    windowFacade->Destroy();
    windowFacade->Destroyed();
}

TEST(WindowFacade, Focus)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"Focus";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());

    {
        std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated())
        {
            gpuFacade->Create();
        }
    }

    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = windowFacade->GetName().data();

        windowFacade->Create(wc);
    }
    EXPECT_TRUE(windowFacade->IsCreated());

    // ウィンドウにフォーカスを当てる
    windowFacade->Focus();

    // フォーカスされたことによる処理を実行する。本来はウィンドウのメッセージループ内で呼ばれるべきだが、テストでは直接呼び出す
    windowFacade->Focused();

    EXPECT_TRUE(windowFacade->IsFocusing());

    // ウィンドウの破棄
    windowFacade->Destroy();
    windowFacade->Destroyed();
}

TEST(WindowFacade, UnFocus)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"UnFocus";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());

    {
        std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated())
        {
            gpuFacade->Create();
        }
    }

    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = windowFacade->GetName().data();

        windowFacade->Create(wc);
    }
    EXPECT_TRUE(windowFacade->IsCreated());

    // ウィンドウのフォーカスを外した際に呼ばれる処理を実行する
    // 本来はウィンドウのメッセージループ内で呼ばれるべきだが、テストでは直接呼び出す
    windowFacade->UnFocused();

    EXPECT_FALSE(windowFacade->IsFocusing());

    // ウィンドウの破棄
    windowFacade->Destroy();
    windowFacade->Destroyed();
}

TEST(WindowFacade, Maximize)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"Maximize";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());

    {
        std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated())
        {
            gpuFacade->Create();
        }
    }

    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = windowFacade->GetName().data();

        windowFacade->Create(wc);
    }
    EXPECT_TRUE(windowFacade->IsCreated());

    // ウィンドウを最大化する
    windowFacade->Maximize();

    // 最大化されたことによる処理を実行する。本来はウィンドウのメッセージループ内で呼ばれるべきだが、テストでは直接呼び出す
    windowFacade->Maximized();

    EXPECT_TRUE(windowFacade->IsMaximizing());

    // ウィンドウの破棄
    windowFacade->Destroy();
    windowFacade->Destroyed();
}

TEST(WindowFacade, Minimize)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"Minimize";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());

    {
        std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated())
        {
            gpuFacade->Create();
        }
    }

    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = windowFacade->GetName().data();

        windowFacade->Create(wc);
    }
    EXPECT_TRUE(windowFacade->IsCreated());

    // ウィンドウを最小化する
    windowFacade->Minimize();

    // 最小化されたことによる処理を実行する。本来はウィンドウのメッセージループ内で呼ばれるべきだが、テストでは直接呼び出す
    windowFacade->Minimized();

    EXPECT_TRUE(windowFacade->IsMinimizing());

    // ウィンドウの破棄
    windowFacade->Destroy();
    windowFacade->Destroyed();
}

TEST(WindowFacade, Restore)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"Restore";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());

    {
        std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated())
        {
            gpuFacade->Create();
        }
    }

    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = windowFacade->GetName().data();

        windowFacade->Create(wc);
    }
    EXPECT_TRUE(windowFacade->IsCreated());

    // ウィンドウをのサイズを戻す
    windowFacade->Restore();

    // 戻されたことによる処理を実行する。本来はウィンドウのメッセージループ内で呼ばれるべきだが、テストでは直接呼び出す
    windowFacade->Restored();

    EXPECT_FALSE(windowFacade->IsMaximizing());
    EXPECT_FALSE(windowFacade->IsMinimizing());

    // ウィンドウの破棄
    windowFacade->Destroy();
    windowFacade->Destroyed();
}

TEST(WindowFacade, FullScreen)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"FullScreen";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());

    {
        std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated())
        {
            gpuFacade->Create();
        }
    }

    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = windowFacade->GetName().data();

        windowFacade->Create(wc);
    }
    EXPECT_TRUE(windowFacade->IsCreated());

    // ウィンドウをフルスクリーンにする
    windowFacade->FullScreen();

    // フルスクリーンされたことによる処理を実行する。本来はウィンドウのメッセージループ内で呼ばれるべきだが、テストでは直接呼び出す
    windowFacade->FullScreened();

    EXPECT_TRUE(windowFacade->IsFullScreen());

    // ウィンドウの破棄
    windowFacade->Destroy();
    windowFacade->Destroyed();
}

TEST(WindowFacade, Move)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"Move";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());

    {
        std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated())
        {
            gpuFacade->Create();
        }
    }

    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = windowFacade->GetName().data();

        windowFacade->Create(wc);
    }
    EXPECT_TRUE(windowFacade->IsCreated());

    // ウィンドウを移動する
    windowFacade->Move(200, 200);

    // 移動されたことによる処理を実行する。本来はウィンドウのメッセージループ内で呼ばれるべきだが、テストでは直接呼び出す
    windowFacade->Moved();

    // ウィンドウの破棄
    windowFacade->Destroy();
    windowFacade->Destroyed();
}

TEST(WindowFacade, ClearAndPresent)
{
    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        window_facade_test::RENDER_TARGET_COUNT, 
        window_facade_test::DEPTH_STENCIL_COUNT
    );

    // WindowContextのSetUp
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"ClearAndPresent";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = 800;
        windowContext->Height() = 600;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのSetUp
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのSetUp
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            window_facade_test::RENDER_TARGET_COUNT, 
            window_facade_test::DEPTH_STENCIL_COUNT
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのSetUp
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            window_facade_test::RENDER_TARGET_COUNT
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // SetUpが完了しているか確認
    EXPECT_TRUE(windowFacade->IsSetUp());

    std::unique_ptr<wb::IGpuFacade> gpuFacade;
    {
        gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated())
        {
            gpuFacade->Create();
        }
    }

    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = windowFacade->GetName().data();

        windowFacade->Create(wc);
    }
    EXPECT_TRUE(windowFacade->IsCreated());

    // コマンドアロケーターとコマンドリストを作成
    ComPtr<ID3D12CommandAllocator> commandAllocator;
    wb::CreateCommandAllocator(gpuFacade->GpuContext()->D3D12Device(), commandAllocator);
    EXPECT_NE(commandAllocator.Get(), nullptr);

    ComPtr<ID3D12GraphicsCommandList> commandList;
    wb::CreateCommandList(gpuFacade->GpuContext()->D3D12Device(), commandAllocator, commandList);
    EXPECT_NE(commandList.Get(), nullptr);

    // ウィンドウをフルスクリーンにする
    windowFacade->FullScreen();

    // フルスクリーンされたことによる処理を実行する。本来はウィンドウのメッセージループ内で呼ばれるべきだが、テストでは直接呼び出す
    windowFacade->FullScreened();

    EXPECT_TRUE(windowFacade->IsFullScreen());

    /*******************************************************************************************************************
     * レンダリング処理
    /******************************************************************************************************************/

    // コマンドをリセット
    wb::ResetCommand(commandList, commandAllocator);

    // レンダーターゲットとして使用するためのバリアを設定
    windowFacade->SetBarrierToRenderTarget(commandList);

    // レンダーターゲットを設定
    windowFacade->SetRenderTarget(commandList);

    // ビューらをクリア
    const float clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // 赤色でクリア
    windowFacade->ClearViews(commandList, clearColor);

    // プレゼントとして使用するためのバリアを設定
    windowFacade->SetBarrierToPresent(commandList);

    // コマンドをクローズ
    wb::CloseCommand(commandList);

    // クローズを待機
    windowFacade->WaitForGpu();

    // コマンドを実行
    std::vector<ID3D12CommandList*> commandLists;
    commandLists.push_back(commandList.Get());
    wb::ExecuteCommand(gpuFacade->GpuContext()->CommandQueue(), commandLists.size(), commandLists);

    // 画面を更新
    windowFacade->Present();

    // フレームを待機
    // ここでフレームインデックスの更新を行っている
    windowFacade->WaitThisFrame();

    // ウィンドウの破棄
    windowFacade->Destroy();
    windowFacade->Destroyed();
}