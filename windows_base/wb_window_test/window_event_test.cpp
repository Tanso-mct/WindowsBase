#include "pch.h"

#include "wb_window/include/window_context.h"
#include "wb_window/include/window_facade.h"
#include "wb_window/include/window_event.h"
#pragma comment(lib, "wb_window.lib")

#include "wb_container/include/dynamic_container.h"

#include "wb_event/include/event_inst_table.h"
#include "wb_event/include/event_func_table.h"
#include "wb_event/include/event_invoker.h"

#include "wb_render_support/include/swap_chain_context.h"
#include "wb_render_support/include/render_target_context.h"
#include "wb_render_support/include/fence_context.h"
#pragma comment(lib, "wb_render_support.lib")

#include "wb_gpu/include/gpu_facade.h"
#pragma comment(lib, "wb_gpu.lib")

#include "wb_monitor/include/keyboard_monitor.h"
#include "wb_monitor/include/keyboard_context.h"
#include "wb_monitor/include/key_code_table.h"
#include "wb_monitor/include/mouse_monitor.h"
#include "wb_monitor/include/mouse_context.h"
#include "wb_monitor/include/mouse_code_table.h"
#include "wb_monitor/include/mouse_input_type_table.h"
#pragma comment(lib, "wb_monitor.lib")

static std::unique_ptr<wb::IWindowEventInvoker> gInvoker;
static std::unique_ptr<wb::ContainerStorage> gContStorage;

static LRESULT CALLBACK WindowProcOnEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // イベント呼び出し
    if (gInvoker != nullptr) gInvoker->Invoke(hWnd, msg, gContStorage, msg, wParam, lParam);

    switch (msg)
    {
    case WM_PAINT:
        // WM_PAINTでDefWindowProcを返すと更新が止まるので、WM_PAINTは0を返す。
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

namespace window_event_test
{
    constexpr UINT RENDER_TARGET_COUNT = 2;
    constexpr UINT DEPTH_STENCIL_COUNT = 1;
} // namespace window_event_test


TEST(WindowEvent, InvokeEvents)
{
    // コンテナストレージのインスタンスを作成
    gContStorage = std::make_unique<wb::ContainerStorage>();

    // WindowFacadeコンテナを作成
    {
        std::unique_ptr<wb::IWindowContainer>& windowCont = gContStorage->Get<wb::IWindowContainer>();
        windowCont = std::make_unique<wb::WindowContainer>();

        windowCont->Create(static_cast<size_t>(wb::WindowID::Size));
    }

    // ウィンドウを作成し、コンテナに渡す
    {
        std::unique_ptr<wb::IWindowFacade> windowFacade = std::make_unique<wb::WindowFacade>
        (
            window_event_test::RENDER_TARGET_COUNT, 
            window_event_test::DEPTH_STENCIL_COUNT
        );

        // WindowContextのSetUp
        {
            std::unique_ptr<wb::IWindowContext> windowContext = std::make_unique<wb::WindowContext>();
            windowContext->Instance() = GetModuleHandle(nullptr);
            windowContext->ParentHandle() = nullptr;
            windowContext->Name() = L"Window Event Invoke Events";
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
                window_event_test::RENDER_TARGET_COUNT
            );
            windowFacade->SetUpSwapChainContext(std::move(swapChainContext));
        }

        // RenderTargetContextのSetUp
        {
            std::unique_ptr<wb::IRenderTargetContext> renderTargetContext = std::make_unique<wb::RenderTargetContext>
            (
                window_event_test::RENDER_TARGET_COUNT, 
                window_event_test::DEPTH_STENCIL_COUNT
            );
            windowFacade->SetUpRenderTargetContext(std::move(renderTargetContext));
        }

        // FenceContextのSetUp
        {
            std::unique_ptr<wb::IFenceContext> fenceContext = std::make_unique<wb::FenceContext>
            (
                window_event_test::RENDER_TARGET_COUNT
            );
            windowFacade->SetUpFenceContext(std::move(fenceContext));
        }

        // SetUpが完了しているか確認
        EXPECT_TRUE(windowFacade->IsSetUp());

        // GpuFacadeが作成されているか確認
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
            wc.lpfnWndProc = WindowProcOnEvent; // イベントを処理するウィンドウプロシージャ
            wc.hInstance = GetModuleHandle(nullptr);
            wc.lpszClassName = windowFacade->GetName().data();

            windowFacade->Create(wc);
        }
        EXPECT_TRUE(windowFacade->IsCreated());

        windowFacade->FullScreen();
        windowFacade->FullScreened();

        // ウィンドウをコンテナに渡す
        std::unique_ptr<wb::IWindowContainer>& windowCont = gContStorage->Get<wb::IWindowContainer>();
        windowCont->Set(wb::WindowID::Main, std::move(windowFacade));
    }

    // モニターコンテナを作成
    {
        std::unique_ptr<wb::IMonitorContainer>& monitorCont = gContStorage->Get<wb::IMonitorContainer>();
        monitorCont = std::make_unique<wb::MonitorContainer>();

        monitorCont->Create(static_cast<size_t>(wb::MonitorID::Size));
    }

    // キーボードモニターを作成し、コンテナに追加
    {
        std::unique_ptr<wb::IKeyboardMonitor> keyboardMonitor = std::make_unique<wb::KeyboardMonitor>();

        {
            std::unique_ptr<wb::IKeyboardContext> context = std::make_unique<wb::KeyboardContext>();
            keyboardMonitor->SetUpContext(std::move(context));
        }

        {
            std::unique_ptr<wb::IKeyCodeTable> keyCodeTable = std::make_unique<wb::KeyCodeTable>();
            keyboardMonitor->SetUpCodeTable(std::move(keyCodeTable));
        }

        // セットアップが完了しているか確認
        EXPECT_TRUE(keyboardMonitor->IsSetUp());

        // キーボードモニターをコンテナに追加
        std::unique_ptr<wb::IMonitorContainer>& monitorCont = gContStorage->Get<wb::IMonitorContainer>();
        monitorCont->Set(wb::MonitorID::MainWindowKeyboard, std::move(keyboardMonitor));
    }

    // マウスモニターを作成し、コンテナに追加
    {
        std::unique_ptr<wb::IMouseMonitor> mouseMonitor = std::make_unique<wb::MouseMonitor>();

        // MouseContextのセットアップ
        {
            std::unique_ptr<wb::IMouseContext> context = std::make_unique<wb::MouseContext>();
            mouseMonitor->SetUpContext(std::move(context));
        }

        // MouseCodeTableのセットアップ
        {
            std::unique_ptr<wb::IMouseCodeTable> mouseCodeTable = std::make_unique<wb::MouseCodeTable>();
            mouseMonitor->SetUpCodeTable(std::move(mouseCodeTable));
        }

        // MouseInputTypeTableのセットアップ
        {
            std::unique_ptr<wb::IInputTypeTable> inputTypeTable = std::make_unique<wb::MouseInputTypeTable>();
            mouseMonitor->SetUpInputTypeTable(std::move(inputTypeTable));
        }

        // セットアップが完了しているか確認
        EXPECT_TRUE(mouseMonitor->IsSetUp());

        // マウスモニターをコンテナに追加
        std::unique_ptr<wb::IMonitorContainer>& monitorCont = gContStorage->Get<wb::IMonitorContainer>();
        monitorCont->Set(wb::MonitorID::MainWindowMouse, std::move(mouseMonitor));
    }

    // インボーカーのインスタンスを作成
    gInvoker = std::make_unique<wb::WindowEventInvoker>();

    // テーブルを設定
    {
        // イベントインスタンステーブルを作成
        std::unique_ptr<wb::IWindowEventInstTable> instTable = std::make_unique<wb::WindowEventInstTable>();

        {
            // WindowEventのインスタンスを作成
            std::unique_ptr<wb::IWindowEvent> windowEvent = std::make_unique<wb::WindowEvent>();

            // ウィンドウが閉じられた時終了するかのフラグを設定
            windowEvent->SetUpNeedToQuitFlag(true);

            // ウィンドウIDを設定
            windowEvent->SetUpWindowID(wb::WindowID::Main);

            // キーボードモニターIDを設定
            windowEvent->SetUpKeyboardMonitorID(wb::MonitorID::MainWindowKeyboard);

            // マウスモニターIDを設定
            windowEvent->SetUpMouseMonitorID(wb::MonitorID::MainWindowMouse);

            //TODO: SceneUpdator及び、ComponentRunnerを使用した処理を実装する

            // SceneUpdatorのSetUp
            {
                // std::unique_ptr<wb::ISceneUpdator> sceneUpdator = std::make_unique<wb::SceneUpdator>();
                // windowEvent->SetUpSceneUpdator(std::move(sceneUpdator));
            }

            // ComponentRunnerコンテナのSetUp
            {
                // std::unique_ptr<wb::IDynamicContainer<wb::IComponentRunner>> compRunnerCont 
                // = std::make_unique<wb::DynamicContainer<wb::IComponentRunner>>();
                // windowEvent->SetUpComponentRunnerCont(std::move(compRunnerCont));
            }

            // SetUpが完了しているか確認
            // EXPECT_TRUE(windowEvent->IsSetUp());

            // WindowFacadeをコンテナから取得
            std::unique_ptr<wb::IWindowContainer>& windowCont = gContStorage->Get<wb::IWindowContainer>();
            std::unique_ptr<wb::IWindowFacade>& windowFacade = windowCont->Get(wb::WindowID::Main);

            // WindowEventをインスタンステーブルに追加
            instTable->Add(windowFacade->GetHandle(), std::move(windowEvent));
        }

        // イベント関数テーブルを作成
        std::unique_ptr<wb::IWindowEventFuncTable> funcTable = std::make_unique<wb::WindowEventFuncTable>();

        {
            // WindowEventの関数をテーブルに追加
            funcTable->Add(WM_KEYDOWN, &wb::IWindowEvent::OnKeyDown);
            funcTable->Add(WM_SYSKEYDOWN, &wb::IWindowEvent::OnKeyDown);
            funcTable->Add(WM_KEYUP, &wb::IWindowEvent::OnKeyUp);
            funcTable->Add(WM_SYSKEYUP, &wb::IWindowEvent::OnKeyUp);

            funcTable->Add(WM_LBUTTONDOWN, &wb::IWindowEvent::OnMouse);
            funcTable->Add(WM_LBUTTONUP, &wb::IWindowEvent::OnMouse);
            funcTable->Add(WM_RBUTTONDOWN, &wb::IWindowEvent::OnMouse);
            funcTable->Add(WM_RBUTTONUP, &wb::IWindowEvent::OnMouse);
            funcTable->Add(WM_MBUTTONDOWN, &wb::IWindowEvent::OnMouse);
            funcTable->Add(WM_MBUTTONUP, &wb::IWindowEvent::OnMouse);
            funcTable->Add(WM_XBUTTONDOWN, &wb::IWindowEvent::OnMouse);
            funcTable->Add(WM_XBUTTONUP, &wb::IWindowEvent::OnMouse);
            funcTable->Add(WM_MOUSEWHEEL, &wb::IWindowEvent::OnMouse);
            funcTable->Add(WM_MOUSEMOVE, &wb::IWindowEvent::OnMouse);

            funcTable->Add(WM_SETFOCUS, &wb::IWindowEvent::OnSetFocus);
            funcTable->Add(WM_KILLFOCUS, &wb::IWindowEvent::OnKillFocus);

            funcTable->Add(WM_SIZE, &wb::IWindowEvent::OnSize);
            funcTable->Add(WM_MOVE, &wb::IWindowEvent::OnMove);
            funcTable->Add(WM_PAINT, &wb::IWindowEvent::OnPaint);

            funcTable->Add(WM_CLOSE, &wb::IWindowEvent::OnClose);
            funcTable->Add(WM_DESTROY, &wb::IWindowEvent::OnDestroy);
        }

        // インボーカーにテーブルを設定
        gInvoker->SetTables(std::move(instTable), std::move(funcTable));
    }

    // メッセージループ
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        // Process any messages in the queue.
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
}