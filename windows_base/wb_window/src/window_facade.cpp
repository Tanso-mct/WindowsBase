#include "wb_window/src/pch.h"
#include "wb_window/include/window_facade.h"

#include "wb_window/include/window_context.h"

#include "wb_gpu/include/gpu_facade.h"
#pragma comment(lib, "wb_gpu.lib")

#include "wb_render_support/include/swap_chain_context.h"
#include "wb_render_support/include/render_target_context.h"
#include "wb_render_support/include/fence_context.h"
#pragma comment(lib, "wb_render_support.lib")

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#include "wb_utility/include/window_helpers.h"
#include "wb_utility/include/dx_helpers.h"
#pragma comment(lib, "wb_utility.lib")

wb::WindowFacade::WindowFacade(UINT renderTargetCount, UINT depthStencilCount) :
    RENDER_TARGET_COUNT(renderTargetCount),
    DEPTH_STENCIL_COUNT(depthStencilCount)
{
}

void wb::WindowFacade::SetUpWindowContext(std::unique_ptr<IWindowContext> windowContext)
{
    if (windowContext_ != nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"WindowContextはすでに設定されています。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    windowContext_ = std::move(windowContext);
}

void wb::WindowFacade::SetUpSwapChainContext(std::unique_ptr<ISwapChainContext> swapChainContext)
{
    if (swapChainContext_ != nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"SwapChainContextはすでに設定されています。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    swapChainContext_ = std::move(swapChainContext);
}

void wb::WindowFacade::SetUpRenderTargetContext(std::unique_ptr<IRenderTargetContext> renderTargetContext)
{
    if (renderTargetContext_ != nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"RenderTargetContextはすでに設定されています。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    renderTargetContext_ = std::move(renderTargetContext);
}

void wb::WindowFacade::SetUpFenceContext(std::unique_ptr<IFenceContext> fenceContext)
{
    if (fenceContext_ != nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"FenceContextはすでに設定されています。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    fenceContext_ = std::move(fenceContext);
}

bool wb::WindowFacade::IsSetUp() const
{
    if (windowContext_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"WindowContextがセットアップされていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        return false;
    }

    if (swapChainContext_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"SwapChainContextがセットアップされていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        return false;
    }

    if (renderTargetContext_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"RenderTargetContextがセットアップされていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        return false;
    }

    if (fenceContext_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"FenceContextがセットアップされていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        return false;
    }

    return true;
}

const HWND &wb::WindowFacade::GetHandle() const
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
    }
    
    return windowContext_->Handle();
}

std::wstring_view wb::WindowFacade::GetName() const
{
    return windowContext_->Name();
}

void wb::WindowFacade::Create(WNDCLASSEX &wc)
{
    /*******************************************************************************************************************
     * ウィンドウの作成
    /******************************************************************************************************************/

    wb::CreateWindowWB
    (
        windowContext_->Handle(), windowContext_->Style(), windowContext_->Name().c_str(),
        windowContext_->PosX(), windowContext_->PosY(),
        windowContext_->Width(), windowContext_->Height(),
        windowContext_->ParentHandle(), wc
    );

    wb::ShowWindowWB(windowContext_->Handle());

    /*******************************************************************************************************************
     * ウィンドウのクライアント領域のサイズを取得
    /******************************************************************************************************************/

    {
        RECT rect;
        GetClientRect(windowContext_->Handle(), &rect);
        windowContext_->ClientWidth() = rect.right - rect.left;
        windowContext_->ClientHeight() = rect.bottom - rect.top;
    }

    /*******************************************************************************************************************
     * GpuContextが作成されているか確認
    /******************************************************************************************************************/

    std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
    if (!gpuFacade->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"GpuContextが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    /*******************************************************************************************************************
     * SwapChainの作成
    /******************************************************************************************************************/

    wb::CreateSwapChain
    (
        gpuFacade->GpuContext()->DXGIFactory(),
        gpuFacade->GpuContext()->CommandQueue(),
        RENDER_TARGET_COUNT,
        swapChainContext_->FrameIndex(),
        windowContext_->Handle(),
        windowContext_->ClientWidth(),
        windowContext_->ClientHeight(),
        swapChainContext_->SwapChain()
    );

    /*******************************************************************************************************************
     * SwapChainのバッファからRenderTargetを取得
    /******************************************************************************************************************/

    wb::GetBuffersFromSwapChain
    (
        swapChainContext_->SwapChain(),
        RENDER_TARGET_COUNT,
        renderTargetContext_->RenderTargets()
    );

    for (UINT i = 0; i < RENDER_TARGET_COUNT; ++i)
    {
        wb::SetName(renderTargetContext_->RenderTargets()[i], L"SwapChain_RenderTarget_" + std::to_wstring(i));
    }

    /*******************************************************************************************************************
     * RenderTargetViewのHeapを作成
    /******************************************************************************************************************/

    wb::CreateRenderTargetViewHeap
    (
        gpuFacade->GpuContext()->D3D12Device(),
        RENDER_TARGET_COUNT, renderTargetContext_->RtvDescriptorHeap(), renderTargetContext_->RtvDescriptorSize()
    );

    /*******************************************************************************************************************
     * RenderTargetViewを作成
    /******************************************************************************************************************/

    wb::CreateRenderTargetView
    (
        gpuFacade->GpuContext()->D3D12Device(),
        RENDER_TARGET_COUNT, renderTargetContext_->RenderTargets(),
        renderTargetContext_->RtvDescriptorHeap(), renderTargetContext_->RtvDescriptorSize()
    );

    /*******************************************************************************************************************
     * CommandAllocatorらの作成
    /******************************************************************************************************************/

    for (UINT i = 0; i < RENDER_TARGET_COUNT; ++i)
    {
        wb::CreateCommandAllocator
        (
            gpuFacade->GpuContext()->D3D12Device(),
            renderTargetContext_->CommandAllocators()[i]
        );
    }

    /*******************************************************************************************************************
     * DepthStencilらの作成
    /******************************************************************************************************************/

    for (UINT i = 0; i < DEPTH_STENCIL_COUNT; ++i)
    {
        wb::CreateDepthStencil
        (
            gpuFacade->GpuContext()->D3D12Device(),
            windowContext_->ClientWidth(), windowContext_->ClientHeight(),
            renderTargetContext_->DepthStencils()[i]
        );
    }

    /*******************************************************************************************************************
     * DepthStencilViewのHeapを作成
    /******************************************************************************************************************/

    wb::CreateDepthStencilViewHeap
    (
        gpuFacade->GpuContext()->D3D12Device(),
        DEPTH_STENCIL_COUNT, renderTargetContext_->DsvDescriptorHeap()
    );

    /*******************************************************************************************************************
     * DepthStencilViewを作成
    /******************************************************************************************************************/

    wb::CreateDepthStencilView
    (
        gpuFacade->GpuContext()->D3D12Device(),
        renderTargetContext_->DepthStencils()[0],
        renderTargetContext_->DsvDescriptorHeap()
    );

    /*******************************************************************************************************************
     * ViewportとScissorRectの設定
    /******************************************************************************************************************/

    wb::CreateViewport
    (
        renderTargetContext_->ViewPort(),
        windowContext_->ClientWidth(), windowContext_->ClientHeight()
    );

    wb::CreateScissorRect
    (
        renderTargetContext_->ScissorRect(),
        windowContext_->ClientWidth(), windowContext_->ClientHeight()
    );

    /*******************************************************************************************************************
     * Fenceらの作成
    /******************************************************************************************************************/

    wb::CreateFence
    (
        gpuFacade->GpuContext()->D3D12Device(),
        fenceContext_->Fence(), wb::DEFAULT_FENCE_VALUE
    );

    wb::CreateFenceEvent(fenceContext_->FenceEvent());

    /*******************************************************************************************************************
     * Windowの作成が終了したとして、フラグを設定
    /******************************************************************************************************************/

    windowContext_->IsCreated() = true;
}

bool wb::WindowFacade::IsCreated() const
{
    return windowContext_->IsCreated();
}

void wb::WindowFacade::Destroy()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    /*******************************************************************************************************************
     * ウィンドウの破棄
    /******************************************************************************************************************/

    DestroyWindow(windowContext_->Handle());
    windowContext_->IsCreated() = false;
}

void wb::WindowFacade::Destroyed()
{
    /*******************************************************************************************************************
     * タスクバーの表示
    /******************************************************************************************************************/

    HWND taskBarHandle = FindWindowW(TASK_BAR_CLASS_NAME, nullptr);
    if (taskBarHandle)
    {
        ShowWindow(taskBarHandle, SW_SHOW);
    }
    else
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"タスクバーが見つかりません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    /*******************************************************************************************************************
     * コンテキストのリセット
    /******************************************************************************************************************/

    windowContext_.reset();
    swapChainContext_.reset();
    renderTargetContext_.reset();
    fenceContext_.reset();
}

void wb::WindowFacade::Resize(UINT width, UINT height)
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    /*******************************************************************************************************************
     * ウィンドウのサイズ変更
    /******************************************************************************************************************/

    if (width == 0 || height == 0)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウのサイズは0以上でなければなりません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    SetWindowPos
    (
        windowContext_->Handle(), nullptr,
        windowContext_->PosX(), windowContext_->PosY(),
        width, height,
        SWP_NOZORDER | SWP_NOACTIVATE
    );
}

void wb::WindowFacade::Resized()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    /*******************************************************************************************************************
     * GpuContextが作成されているか確認
    /******************************************************************************************************************/

    std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
    if (!gpuFacade->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"GpuContextが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    /*******************************************************************************************************************
     * クライアント領域のサイズを更新
    /******************************************************************************************************************/

    {
        RECT rect;
        GetClientRect(windowContext_->Handle(), &rect);

        if 
        (
            windowContext_->ClientWidth() == rect.right - rect.left &&
            windowContext_->ClientHeight() == rect.bottom - rect.top
        ){
            // サイズが変更されていない場合は何もしない
            return;
        }

        windowContext_->ClientWidth() = rect.right - rect.left;
        windowContext_->ClientHeight() = rect.bottom - rect.top;
    }

    /*******************************************************************************************************************
     * RenderTargetとDepthStencilを解放
    /******************************************************************************************************************/

    for (UINT i = 0; i < RENDER_TARGET_COUNT; ++i)
    {
        renderTargetContext_->RenderTargets()[i].Reset();
        renderTargetContext_->CommandAllocators()[i].Reset();
    }

    for (UINT i = 0; i < DEPTH_STENCIL_COUNT; ++i)
    {
        renderTargetContext_->DepthStencils()[i].Reset();
    }

    /*******************************************************************************************************************
     * SwapChainのサイズを変更
    /******************************************************************************************************************/

    wb::ResizeSwapChain
    (
        swapChainContext_->FrameCount(), swapChainContext_->FrameIndex(),
        windowContext_->ClientWidth(), windowContext_->ClientHeight(),
        swapChainContext_->SwapChain()
    );

    /*******************************************************************************************************************
     * SwapChainのバッファからRenderTargetを再取得
    /******************************************************************************************************************/

    wb::GetBuffersFromSwapChain
    (
        swapChainContext_->SwapChain(),
        RENDER_TARGET_COUNT,
        renderTargetContext_->RenderTargets()
    );

    /*******************************************************************************************************************
     * RenderTargetViewを再作成
    /******************************************************************************************************************/

    wb::CreateRenderTargetView
    (
        gpuFacade->GpuContext()->D3D12Device(),
        RENDER_TARGET_COUNT, renderTargetContext_->RenderTargets(),
        renderTargetContext_->RtvDescriptorHeap(), renderTargetContext_->RtvDescriptorSize()
    );

    /*******************************************************************************************************************
     * DepthStencilを再作成
    /******************************************************************************************************************/

    for (UINT i = 0; i < DEPTH_STENCIL_COUNT; ++i)
    {
        wb::CreateDepthStencil
        (
            gpuFacade->GpuContext()->D3D12Device(),
            windowContext_->ClientWidth(), windowContext_->ClientHeight(),
            renderTargetContext_->DepthStencils()[i]
        );
    }

    /*******************************************************************************************************************
     * DepthStencilViewを再作成
    /******************************************************************************************************************/

    wb::CreateDepthStencilView
    (
        gpuFacade->GpuContext()->D3D12Device(),
        renderTargetContext_->DepthStencils()[0],
        renderTargetContext_->DsvDescriptorHeap()
    );

    /*******************************************************************************************************************
     * ViewportとScissorRectの再設定
    /******************************************************************************************************************/

    wb::CreateViewport
    (
        renderTargetContext_->ViewPort(),
        windowContext_->ClientWidth(), windowContext_->ClientHeight()
    );

    wb::CreateScissorRect
    (
        renderTargetContext_->ScissorRect(),
        windowContext_->ClientWidth(), windowContext_->ClientHeight()
    );
}

bool wb::WindowFacade::IsFocusing() const
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return false;
    }

    return windowContext_->IsFocus();
}

void wb::WindowFacade::Focus()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    ShowWindow(windowContext_->Handle(), SW_SHOW);
    SetForegroundWindow(windowContext_->Handle());
    SetFocus(windowContext_->Handle());
}

void wb::WindowFacade::Focused()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    windowContext_->IsFocus() = true;

    // フォーカスを得たとき、フルスクリーンの場合はタスクバーを非表示にする
    HWND taskbarHandle = FindWindow(TASK_BAR_CLASS_NAME, nullptr);
    if (taskbarHandle && windowContext_->IsFullScreen())
    {
        ShowWindow(taskbarHandle, SW_HIDE);
    }
    else if (!taskbarHandle)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"タスクバーのハンドルが取得できません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }
}

void wb::WindowFacade::UnFocused()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    windowContext_->IsFocus() = false;

    // ウィンドウがフォーカスを失ったときは必ずタスクバーを表示する
    HWND taskbarHandle = FindWindow(TASK_BAR_CLASS_NAME, nullptr);
    if (taskbarHandle)
    {
        ShowWindow(taskbarHandle, SW_SHOW);
    }
    else
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"タスクバーのハンドルが取得できません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }
}

bool wb::WindowFacade::IsMaximizing() const
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return false;
    }

    return windowContext_->IsMaximized();
}

void wb::WindowFacade::Maximize()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    ShowWindow(windowContext_->Handle(), SW_MAXIMIZE);

    // ウィンドウのスタイルを設定
    SetWindowLong
    (
        windowContext_->Handle(), GWL_STYLE,
        windowContext_->Style() | WS_VISIBLE
    );

}

void wb::WindowFacade::Maximized()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    windowContext_->IsMaximized() = true;
    windowContext_->IsMinimized() = false;
    windowContext_->IsFullScreen() = false;

    Resized();
}

bool wb::WindowFacade::IsMinimizing() const
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return false;
    }

    return windowContext_->IsMinimized();
}

void wb::WindowFacade::Minimize()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    ShowWindow(windowContext_->Handle(), SW_MINIMIZE);

    // ウィンドウのスタイルを設定
    SetWindowLong
    (
        windowContext_->Handle(), GWL_STYLE,
        windowContext_->Style() | WS_VISIBLE
    );

}

void wb::WindowFacade::Minimized()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    windowContext_->IsMinimized() = true;
    windowContext_->IsMaximized() = false;
    windowContext_->IsFullScreen() = false;
}

bool wb::WindowFacade::IsFullScreen() const
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return false;
    }

    return windowContext_->IsFullScreen();
}

void wb::WindowFacade::FullScreen()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    UINT screenWidth, screenHeight;
    wb::GetCurrentScreenSize(windowContext_->Handle(), screenWidth, screenHeight);

    // ウィンドウのスタイルをフルスクリーンに変更
    SetWindowLong(windowContext_->Handle(), GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos
    (
        windowContext_->Handle(), nullptr,
        0, 0, screenWidth, screenHeight,
        SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED
    );

    // タスクバーを隠す
    HWND taskbarHandle = FindWindow(TASK_BAR_CLASS_NAME, nullptr);
    if (taskbarHandle)
    {
        ShowWindow(taskbarHandle, SW_HIDE);
    }
    else
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"タスクバーのハンドルが取得できません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

}

void wb::WindowFacade::FullScreened()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    windowContext_->IsFullScreen() = true;
    windowContext_->IsMaximized() = false;
    windowContext_->IsMinimized() = false;

    Resized();
}

void wb::WindowFacade::Restore()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    ShowWindow(windowContext_->Handle(), SW_RESTORE);

    // ウィンドウのスタイルを設定
    SetWindowLong
    (
        windowContext_->Handle(), GWL_STYLE,
        windowContext_->Style() | WS_VISIBLE
    );

    // タスクバーを再表示
    HWND taskbarHandle = FindWindow(TASK_BAR_CLASS_NAME, nullptr);
    if (taskbarHandle)
    {
        ShowWindow(taskbarHandle, SW_SHOW);
    }
    else
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"タスクバーのハンドルが取得できません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }
}

void wb::WindowFacade::Restored()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    windowContext_->IsMaximized() = false;
    windowContext_->IsMinimized() = false;
    windowContext_->IsFullScreen() = false;

    Resized();
}

void wb::WindowFacade::Move(UINT posX, UINT posY)
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    SetWindowPos
    (
        windowContext_->Handle(), nullptr,
        posX, posY,
        windowContext_->Width(), windowContext_->Height(),
        SWP_NOZORDER | SWP_NOACTIVATE
    );
}

void wb::WindowFacade::Moved()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    // ウィンドウの位置を更新
    RECT rect;
    GetWindowRect(windowContext_->Handle(), &rect);
    windowContext_->PosX() = rect.left;
    windowContext_->PosY() = rect.top;
}

void wb::WindowFacade::SetBarrierToRenderTarget(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList)
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    wb::SetBarrier
    (
        cmdList, renderTargetContext_->RenderTargets()[swapChainContext_->FrameIndex()],
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
    );
}

void wb::WindowFacade::SetBarrierToPresent(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList)
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    wb::SetBarrier
    (
        cmdList, renderTargetContext_->RenderTargets()[swapChainContext_->FrameIndex()],
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
    );
}

void wb::WindowFacade::SetRenderTarget(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList)
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    wb::SetRenderTargets
    (
        cmdList,
        renderTargetContext_->RtvDescriptorHeap(),
        swapChainContext_->FrameIndex(), renderTargetContext_->RtvDescriptorSize(),
        renderTargetContext_->DsvDescriptorHeap(),
        0, renderTargetContext_->DsvDescriptorSize()
    );

    cmdList->RSSetViewports(1, &renderTargetContext_->ViewPort());
    cmdList->RSSetScissorRects(1, &renderTargetContext_->ScissorRect());

}

void wb::WindowFacade::ClearViews
(
    const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList, const float (&clearColor)[4]
){
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    // RenderTargetViewのクリア
    wb::ClearRenderTargetViews
    (
        cmdList,
        renderTargetContext_->RtvDescriptorHeap(),
        swapChainContext_->FrameIndex(), renderTargetContext_->RtvDescriptorSize(),
        clearColor
    );

    // DepthStencilViewのクリア
    wb::ClearDepthStencilView
    (
        cmdList,
        renderTargetContext_->DsvDescriptorHeap(),
        0, renderTargetContext_->DsvDescriptorSize(),
        renderTargetContext_->ScissorRect()
    );
    
}

void wb::WindowFacade::Present()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    // SwapChainのPresentを呼び出す
    swapChainContext_->SwapChain()->Present(1, 0);
}

void wb::WindowFacade::WaitForGpu()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    // GpuContextが作成されているか確認
    std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
    if (!gpuFacade->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"GpuContextが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    wb::WaitForGPU
    (
        gpuFacade->GpuContext()->CommandQueue(),
        fenceContext_->Fence(), 
        fenceContext_->FenceValues()[swapChainContext_->FrameIndex()], fenceContext_->FenceEvent()
    );
}

void wb::WindowFacade::WaitThisFrame()
{
    if (!windowContext_->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"ウィンドウが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    // GpuContextが作成されているか確認
    std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
    if (!gpuFacade->IsCreated())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"GpuContextが作成されていません。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
        return;
    }

    // 現在のフレームのFence値を取得
    UINT64 fenceValue = fenceContext_->FenceValues()[swapChainContext_->FrameIndex()];

    // フレームインデックスを更新
    swapChainContext_->FrameIndex() = swapChainContext_->SwapChain()->GetCurrentBackBufferIndex();

    wb::WaitFrameForGPU
    (
        gpuFacade->GpuContext()->CommandQueue(),
        fenceContext_->Fence(), fenceContext_->FenceEvent(),
        fenceValue, fenceContext_->FenceValues()[swapChainContext_->FrameIndex()]
    );

}
