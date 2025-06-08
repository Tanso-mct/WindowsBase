#include "integration_test/src/pch.h"

#include "wb_container/include/container_storage.h"

#include "wb_window/include/window_collection.h"
#include "wb_window/include/window_event.h"
#pragma comment(lib, "wb_window.lib")

#include "wb_render_support/include/swap_chain_context.h"
#include "wb_render_support/include/render_target_context.h"
#include "wb_render_support/include/fence_context.h"
#pragma comment(lib, "wb_render_support.lib")

// Utilityのヘッダーはプリコンパイル済みヘッダーに含まれている
#pragma comment(lib, "wb_utility.lib")

#include "wb_monitor/include/monitor_collection.h"
#pragma comment(lib, "wb_monitor.lib")

#include "wb_gpu/include/gpu_facade.h"
#pragma comment(lib, "wb_gpu.lib")

#include "wb_event/include/event_inst_table.h"
#include "wb_event/include/event_func_table.h"
#include "wb_event/include/event_invoker.h"


// Global変数らを定義
static std::unique_ptr<wb::ContainerStorage> gContStorage;
static std::unique_ptr<wb::IWindowEventInvoker> gInvoker;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // イベント呼び出し
    if (gInvoker) gInvoker->Invoke(hWnd, msg, gContStorage, msg, wParam, lParam);

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

int APIENTRY wWinMain
(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
){
    /*******************************************************************************************************************
     * Debug時のみ、コンソールを表示する
    /******************************************************************************************************************/
    
#ifndef NDEBUG
    wb::CreateStandardConsole();
#endif // !NDEBUG

    /*******************************************************************************************************************
     * コンテナのセットアップ
    /******************************************************************************************************************/

    gContStorage = std::make_unique<wb::ContainerStorage>();

    // WindowFacadeのコンテナを作成
    {
        std::unique_ptr<wb::IWindowContainer> &windowCont = gContStorage->Get<wb::IWindowContainer>();
        windowCont = std::make_unique<wb::WindowContainer>();

        windowCont->Create(static_cast<size_t>(wb::WindowID::Size));
    }

    // SceneFacadeのコンテナを作成
    {
        std::unique_ptr<wb::ISceneContainer> &sceneCont = gContStorage->Get<wb::ISceneContainer>();
        sceneCont = std::make_unique<wb::SceneContainer>();

        sceneCont->Create(static_cast<size_t>(wb::SceneID::Size));
    }

    // CommonFacadeのコンテナを作成
    {
        std::unique_ptr<wb::ICommonContainer> &commonCont = gContStorage->Get<wb::ICommonContainer>();
        commonCont = std::make_unique<wb::CommonContainer>();

        commonCont->Create(static_cast<size_t>(wb::CommonID::Size));
    }

    // Monitorのコンテナを作成
    {
        std::unique_ptr<wb::IMonitorContainer> &monitorCont = gContStorage->Get<wb::IMonitorContainer>();
        monitorCont = std::make_unique<wb::MonitorContainer>();

        monitorCont->Create(static_cast<size_t>(wb::MonitorID::Size));
    }

    // Assetのコンテナを作成
    {
        std::unique_ptr<wb::IAssetContainer> &assetCont = gContStorage->Get<wb::IAssetContainer>();
        assetCont = std::make_unique<wb::AssetContainer>();

        assetCont->Create(static_cast<size_t>(wb::AssetType::Size));

        // TODO: AssetTypeごとのコンテナを作成する
    }

    /*******************************************************************************************************************
     * Windowをセットアップし、コンテナに渡す
    /******************************************************************************************************************/

    {
        std::unique_ptr<wb::IWindowContainer> &windowCont = gContStorage->Get<wb::IWindowContainer>();
        wb::WindowFacadeCollection windowFacadeCollection;

        // MainWindow
        {
            std::unique_ptr<wb::IWindowFacadeFactory> &factory = windowFacadeCollection.GetFactory(wb::WindowID::Main);
            std::unique_ptr<wb::IWindowFacade> windowFacade = factory->Create();

            windowCont->Set(wb::WindowID::Main, std::move(windowFacade));
        }
    }

    /*******************************************************************************************************************
     * Sceneをセットアップし、コンテナに渡す
    /******************************************************************************************************************/

    /*******************************************************************************************************************
     * Commonをセットアップし、コンテナに渡す
    /******************************************************************************************************************/

    /*******************************************************************************************************************
     * Monitorをセットアップし、コンテナに渡す
    /******************************************************************************************************************/

    {
        std::unique_ptr<wb::IMonitorContainer> &monitorCont = gContStorage->Get<wb::IMonitorContainer>();
        wb::MonitorCollection monitorCollection;

        // キーボードモニターを作成し、コンテナに追加
        {
            std::unique_ptr<wb::IMonitorFactory> &factory 
            = monitorCollection.GetFactory(wb::MonitorID::MainWindowKeyboard);
            std::unique_ptr<wb::IMonitor> monitor = factory->Create();

            monitorCont->Set(wb::MonitorID::MainWindowKeyboard, std::move(monitor));
        }

        // マウスモニターを作成し、コンテナに追加
        {
            std::unique_ptr<wb::IMonitorFactory> &factory 
            = monitorCollection.GetFactory(wb::MonitorID::MainWindowMouse);
            std::unique_ptr<wb::IMonitor> monitor = factory->Create();

            monitorCont->Set(wb::MonitorID::MainWindowMouse, std::move(monitor));
        }
    }

    /*******************************************************************************************************************
     * GpuFacadeの作成
    /******************************************************************************************************************/

    {
        std::unique_ptr<wb::IGpuFacade> gpuFacade = std::make_unique<wb::GpuFacade>();
        if (!gpuFacade->IsCreated()) gpuFacade->Create();
    }

    /*******************************************************************************************************************
     * Windowの作成
    /******************************************************************************************************************/

    {
        std::unique_ptr<wb::IWindowContainer> &windowCont = gContStorage->Get<wb::IWindowContainer>();

        // MainWindow
        {
            std::unique_ptr<wb::IWindowFacade> &windowFacade = windowCont->Get(wb::WindowID::Main);

            WNDCLASSEX wc = {};
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.lpfnWndProc = WindowProc;
            wc.hInstance = GetModuleHandle(nullptr);
            wc.lpszClassName = windowFacade->GetName().data();

            windowFacade->Create(wc);
        }
    }

    /*******************************************************************************************************************
     * Windowのイベントをセットアップ
    /******************************************************************************************************************/

    gInvoker = std::make_unique<wb::WindowEventInvoker>();

    // テーブルを設定
    {
        // イベントインスタンステーブルを作成
        std::unique_ptr<wb::IWindowEventInstTable> instTable = std::make_unique<wb::WindowEventInstTable>();

        wb::WindowEventCollection windowEventCollection;
        std::unique_ptr<wb::IWindowContainer> &windowCont = gContStorage->Get<wb::IWindowContainer>();

        // MainWindow
        {
            std::unique_ptr<wb::IWindowEventFactory> &factory = windowEventCollection.GetFactory(wb::WindowID::Main);
            std::unique_ptr<wb::IWindowEvent> windowEvent = factory->Create();

            // ウィンドウのハンドルを取得し、イベントインスタンステーブルに追加
            std::unique_ptr<wb::IWindowFacade> &windowFacade = windowCont->Get(wb::WindowID::Main);

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

    /*******************************************************************************************************************
     * Windowメッセージループ
    /******************************************************************************************************************/

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}