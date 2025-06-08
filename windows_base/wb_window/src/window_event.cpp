#include "wb_window/src/pch.h"
#include "wb_window/include/window_event.h"

#include "wb_utility/include/error_handler.h"
#include "wb_utility/include/console_log.h"
#include "wb_utility/include/type_cast.h"
#pragma comment(lib, "wb_utility.lib")

std::unique_ptr<wb::IWindowFacade> &wb::WindowEvent::GetWindowFacade
(
    const std::unique_ptr<ContainerStorage> &contStorage
){
    // コンテナストレージからウィンドウコンテナを取得
    std::unique_ptr<wb::IWindowContainer> &windowCont = contStorage->Get<wb::IWindowContainer>();
    if (!windowCont)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"MainWindowEvent GetWindowFacade : WindowContainerが存在しません"}
        );
        wb::ErrorNotify("WB_WINDOW", err);
        wb::QuitProgram();
        return windowCont->Get(thisID_); // ここでnullptrが返る可能性がある
    }

    // ウィンドウコンテナからウィンドウファサードを取得
    std::unique_ptr<wb::IWindowFacade> &windowFacade = windowCont->Get(thisID_);
    if (!windowFacade)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"MainWindowEvent GetWindowFacade : WindowFacadeがコンテナに存在しません"}
        );
        wb::ErrorNotify("WB_WINDOW", err);
        wb::QuitProgram();
    }

    return windowFacade;
}

bool wb::WindowEvent::IsDestroying()
{
    return isDestroying_;
}

void wb::WindowEvent::SetUpNeedToQuitFlag(bool needToQuitFlag)
{
    needToQuit_ = needToQuitFlag;
}

void wb::WindowEvent::SetUpWindowID(WindowID windowID)
{
    thisID_ = windowID;
}

void wb::WindowEvent::SetUpKeyboardMonitorID(MonitorID monitorID)
{
    keyboardMonitorID_ = monitorID;
}

void wb::WindowEvent::SetUpMouseMonitorID(MonitorID monitorID)
{
    mouseMonitorID_ = monitorID;
}

void wb::WindowEvent::SetUpSceneUpdator(std::unique_ptr<ISceneUpdator> sceneUpdator)
{
    sceneUpdator_ = std::move(sceneUpdator);
}

void wb::WindowEvent::SetUpComponentRunnerCont(std::unique_ptr<IDynamicContainer<IComponentRunner>> compRunnerCont)
{
    compRunnerCont_ = std::move(compRunnerCont);
}

bool wb::WindowEvent::IsSetUp() const
{
    if (sceneUpdator_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"MainWindowEvent IsSetUpped : SceneUpdatorがセットアップされていません"}
        );
        wb::ErrorNotify("WB_WINDOW", err);
        return false;
    }

    if (compRunnerCont_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"MainWindowEvent IsSetUpped : ComponentRunnerContがセットアップされていません"}
        );
        wb::ErrorNotify("WB_WINDOW", err);
        return false;
    }

    return true;
}

void wb::WindowEvent::OnKeyDown(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::unique_ptr<wb::IMonitorContainer> &monitorCont = contStorage->Get<wb::IMonitorContainer>();
    std::unique_ptr<wb::IMonitor> &monitor = monitorCont->Get(keyboardMonitorID_);

    {
        wb::CastWithReturn<wb::IMonitor, wb::IKeyboardMonitor> keyboardMonitor(monitor);
        keyboardMonitor()->EditStateByDown(wParam, lParam);
    }
}

void wb::WindowEvent::OnKeyUp(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::unique_ptr<wb::IMonitorContainer> &monitorCont = contStorage->Get<wb::IMonitorContainer>();
    std::unique_ptr<wb::IMonitor> &monitor = monitorCont->Get(keyboardMonitorID_);

    {
        wb::CastWithReturn<wb::IMonitor, wb::IKeyboardMonitor> keyboardMonitor(monitor);
        keyboardMonitor()->EditStateByUp(wParam, lParam);
    }
}

void wb::WindowEvent::OnMouse(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::unique_ptr<wb::IMonitorContainer> &monitorCont = contStorage->Get<wb::IMonitorContainer>();
    std::unique_ptr<wb::IMonitor> &monitor = monitorCont->Get(mouseMonitorID_);

    {
        wb::CastWithReturn<wb::IMonitor, wb::IMouseMonitor> mouseMonitor(monitor);
        mouseMonitor()->EditState(msg, wParam, lParam);
    }
}

void wb::WindowEvent::OnSetFocus(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::unique_ptr<wb::IWindowFacade> &windowFacade = GetWindowFacade(contStorage);
    windowFacade->Focused();
}

void wb::WindowEvent::OnKillFocus(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::unique_ptr<wb::IWindowFacade> &windowFacade = GetWindowFacade(contStorage);
    windowFacade->UnFocused();
}

void wb::WindowEvent::OnSize(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::unique_ptr<wb::IWindowFacade> &windowFacade = GetWindowFacade(contStorage);
    
    if (wParam == SIZE_RESTORED) windowFacade->Restored();
    else if (wParam == SIZE_MAXIMIZED) windowFacade->Maximized();
    else if (wParam == SIZE_MINIMIZED) windowFacade->Minimized();
}

void wb::WindowEvent::OnMove(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::unique_ptr<wb::IWindowFacade> &windowFacade = GetWindowFacade(contStorage);
    windowFacade->Moved();
}

void wb::WindowEvent::OnPaint(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //TODO: SceneUpdator、各ComponentRunnerらの処理を実装する

    /*******************************************************************************************************************
     * KeyboardMonitorの使用例
    /******************************************************************************************************************/

    {
        std::unique_ptr<wb::IMonitorContainer> &monitorCont = contStorage->Get<wb::IMonitorContainer>();
        std::unique_ptr<wb::IMonitor> &monitor = monitorCont->Get(keyboardMonitorID_);

        {
            wb::CastWithReturn<wb::IMonitor, wb::IKeyboardMonitor> keyboardMonitor(monitor);

            if (keyboardMonitor()->GetKey(wb::KeyCode::W)) wb::ConsoleLog({"Wキーが押されています"});
            if (keyboardMonitor()->GetKeyDown(wb::KeyCode::A)) wb::ConsoleLog({"Aキーが押された瞬間です"});
            if (keyboardMonitor()->GetKeyUp(wb::KeyCode::S)) wb::ConsoleLog({"Sキーが離された瞬間です"});
            if (keyboardMonitor()->GetKeyDoubleTap(wb::KeyCode::D, 0.2)) 
            {
                wb::ConsoleLog({"Dキーがダブルタップされました"});
            }

            if (keyboardMonitor()->GetKeyDown(wb::KeyCode::Escape)) wb::ConsoleLog({"Escapeキーが押された瞬間です"});
            if (keyboardMonitor()->GetKeyDown(wb::KeyCode::Return)) wb::ConsoleLog({"Returnキーが押された瞬間です"});
            if (keyboardMonitor()->GetKeyDown(wb::KeyCode::LAlt)) wb::ConsoleLog({"左Altキーが押された瞬間です"});
            if (keyboardMonitor()->GetKeyDown(wb::KeyCode::RAlt)) wb::ConsoleLog({"右Altキーが押された瞬間です"});
            if (keyboardMonitor()->GetKeyDown(wb::KeyCode::LShift)) wb::ConsoleLog({"左Shiftキーが押された瞬間です"});  
            if (keyboardMonitor()->GetKeyDown(wb::KeyCode::RShift)) wb::ConsoleLog({"右Shiftキーが押された瞬間です"});

            if (keyboardMonitor()->GetKeyDown(wb::KeyCode::F1)) wb::ConsoleLog({"F1キーが押された瞬間です"});
            if (keyboardMonitor()->GetKeyDown(wb::KeyCode::F2)) wb::ConsoleLog({"F2キーが押された瞬間です"});
        }
    }

    /*******************************************************************************************************************
     * MouseMonitorの使用例
    /******************************************************************************************************************/

    {
        std::unique_ptr<wb::IMonitorContainer> &monitorCont = contStorage->Get<wb::IMonitorContainer>();
        std::unique_ptr<wb::IMonitor> &monitor = monitorCont->Get(mouseMonitorID_);

        {
            wb::CastWithReturn<wb::IMonitor, wb::IMouseMonitor> mouseMonitor(monitor);

            if (mouseMonitor()->GetButton(wb::MouseCode::Left)) 
            {
                wb::ConsoleLog({"左クリックが押されています"});
            }

            if (mouseMonitor()->GetButtonDown(wb::MouseCode::Right)) 
            {
                wb::ConsoleLog({"右クリックが押された瞬間です"});
            }

            if (mouseMonitor()->GetButtonUp(wb::MouseCode::Middle)) 
            {
                wb::ConsoleLog({"中クリックが離された瞬間です"});
            }

            if (mouseMonitor()->GetButtonDoubleTap(wb::MouseCode::Right, 0.2))
            {
                wb::ConsoleLog({"右クリックがダブルタップされました"});
            }

            if (mouseMonitor()->GetButtonDown(wb::MouseCode::X1)) 
            {
                wb::ConsoleLog({"XButton1が押された瞬間です"});
            }

            if (mouseMonitor()->GetButtonDown(wb::MouseCode::X2)) 
            {
                wb::ConsoleLog({"XButton2が押された瞬間です"});
            }

            if (mouseMonitor()->GetScrollDelta() != 0) 
            {
                std::string scrollDelta = "スクロール量: " + std::to_string(mouseMonitor()->GetScrollDelta());
                wb::ConsoleLog
                ({
                    "マウスホイールが動かされました",
                    scrollDelta.c_str()
                });
            }

            UINT posX = mouseMonitor()->GetPositionX();
            UINT posY = mouseMonitor()->GetPositionY();
            std::string posStr = "マウスの位置: (" + std::to_string(posX) + ", " + std::to_string(posY) + ")";
            // wb::ConsoleLog({posStr.c_str()});
        }
    }

    /*******************************************************************************************************************
     * KeyboardMonitor、MouseMonitorの状態を更新する
    /******************************************************************************************************************/

    {
        std::unique_ptr<wb::IMonitorContainer> &monitorCont = contStorage->Get<wb::IMonitorContainer>();

        // KeyboardMonitor
        std::unique_ptr<wb::IMonitor> &keyboardMonitor = monitorCont->Get(keyboardMonitorID_);
        keyboardMonitor->UpdateState();

        // MouseMonitor
        std::unique_ptr<wb::IMonitor> &mouseMonitor = monitorCont->Get(mouseMonitorID_);
        mouseMonitor->UpdateState();
    }

}

void wb::WindowEvent::OnClose(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //TODO: SceneUpdator、各ComponentRunnerらの処理を実装する
}

void wb::WindowEvent::OnDestroy(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam)
{
    std::unique_ptr<wb::IWindowFacade> &windowFacade = GetWindowFacade(contStorage);
    windowFacade->Destroyed();
    isDestroying_ = true;

    if (needToQuit_) PostQuitMessage(0);
}