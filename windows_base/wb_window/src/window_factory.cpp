#include "wb_window/src/pch.h"
#include "wb_window/include/window_factory.h"

#include "wb_window/include/window_facade.h"
#include "wb_window/include/window_context.h"
#include "wb_window/include/window_event.h"

#include "wb_render_support/include/swap_chain_context.h"
#include "wb_render_support/include/render_target_context.h"
#include "wb_render_support/include/fence_context.h"
#pragma comment(lib, "wb_render_support.lib")

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

std::unique_ptr<wb::IWindowFacade> wb::MainWindowFacadeFactory::Create()
{
    const UINT renderTargetCount = 2;
    const UINT depthStencilCount = 1;

    std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
    (
        renderTargetCount, depthStencilCount
    );

    // WindowContextのセットアップ
    {
        std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
        windowContext->Instance() = GetModuleHandle(nullptr);
        windowContext->ParentHandle() = nullptr;
        windowContext->Name() = L"BDC Game";
        windowContext->PosX() = CW_USEDEFAULT;
        windowContext->PosY() = CW_USEDEFAULT;
        windowContext->Width() = wb::DEFAULT_WINDOW_WIDTH;
        windowContext->Height() = wb::DEFAULT_WINDOW_HEIGHT;

        windowFacade->SetUpWindowContext(std::move(windowContext));
    }

    // SwapChainContextのセットアップ
    {
        std::unique_ptr<wb::ISwapChainContext> swapChainContext = std::make_unique<wb::SwapChainContext>
        (
            renderTargetCount
        );
        windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
    }

    // RenderTargetContextのセットアップ
    {
        std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
        (
            renderTargetCount, depthStencilCount
        );
        windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
    }

    // FenceContextのセットアップ
    {
        std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
        (
            renderTargetCount
        );
        windowFacade->SetUpFenceContext(std::move(fenceContext));
    }

    // セットアップが完了しているか確認
    if (!windowFacade->IsSetUp())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"WindowFacadeのセットアップに失敗しました。"}
        );
        wb::ErrorNotify("WINDOW_FACADE", err);
        wb::QuitProgram();
    }

    return windowFacade;
}

std::unique_ptr<wb::IWindowEvent> wb::MainWindowEventFactory::Create()
{
    std::unique_ptr<wb::IWindowEvent> windowEvent = std::make_unique<wb::WindowEvent>();

    // ウィンドウが閉じられた時、プログラムを終了するように設定
    windowEvent->SetUpNeedToQuitFlag(true);

    // ウィンドウIDをメインウィンドウに設定
    windowEvent->SetUpWindowID(wb::WindowID::Main);

    // キーボードモニターIDをメインウィンドウのキーボードに設定
    windowEvent->SetUpKeyboardMonitorID(wb::MonitorID::MainWindowKeyboard);

    // マウスモニターIDをメインウィンドウのマウスに設定
    windowEvent->SetUpMouseMonitorID(wb::MonitorID::MainWindowMouse);

    // SceneUpdatorのセットアップ
    {
        //TODO: SceneUpdatorをWindowEventに渡す処理を実装する
    }

    // ComponentRunnerのコンテナをセットアップ
    {
        //TODO: ComponentRunnerのコンテナをWindowEventに渡す処理を実装する
    }

    // セットアップが完了しているか確認
    //TODO: WindowEventのセットアップが完了しているか確認する処理のコメントアウトを無効化する
    // if (!windowEvent->IsSetUp())
    // {
    //     std::string err = wb::ConsoleLogErr
    //     (
    //         __FILE__, __LINE__, __FUNCTION__,
    //         {"WindowEventのセットアップに失敗しました。"}
    //     );
    //     wb::ErrorNotify("WINDOW_EVENT", err);
    //     wb::QuitProgram();
    // }

    return windowEvent;
}
