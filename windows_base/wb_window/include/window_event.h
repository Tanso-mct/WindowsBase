#pragma once

#include "wb_window/include/dll_config.h"

#include "wb_interface/event.h"
#include "wb_interface/window.h"
#include "wb_interface/scene.h"
#include "wb_interface/container.h"
#include "wb_interface/component.h"

#include "wb_event/include/event_invoker.h"
#include "wb_event/include/event_inst_table.h"
#include "wb_event/include/event_func_table.h"
#include "wb_container/include/container_storage.h"

namespace wb
{
    /*******************************************************************************************************************
     * WindowEventをEventInvoker, EventInstTable, EventFuncTableのテンプレートパラメータとして使用するための型定義
     * これにより、WindowEventをイベントで使用できるようにする
    /******************************************************************************************************************/

    using IWindowEventInvoker = wb::IEventInvoker
    <
        HWND, wb::IWindowEvent, UINT, const std::unique_ptr<wb::ContainerStorage>&, UINT, WPARAM, LPARAM
    >;
    using WindowEventInvoker = wb::EventInvoker
    <
        HWND, wb::IWindowEvent, UINT, const std::unique_ptr<wb::ContainerStorage>&, UINT, WPARAM, LPARAM
    >;

    using IWindowEventInstTable = wb::IEventInstTable<HWND, wb::IWindowEvent>;
    using WindowEventInstTable = wb::EventInstTable<HWND, wb::IWindowEvent>;

    using IWindowEventFuncTable = wb::IEventFuncTable
    <
        UINT, wb::IWindowEvent, const std::unique_ptr<wb::ContainerStorage>&, UINT, WPARAM, LPARAM
    >;
    using WindowEventFuncTable = wb::EventFuncTable
    <
        UINT, wb::IWindowEvent, const std::unique_ptr<wb::ContainerStorage>&, UINT, WPARAM, LPARAM
    >;

    /*******************************************************************************************************************
    /******************************************************************************************************************/

    class WB_WINDOW_API WindowEvent : public IWindowEvent
    {
    private:
        bool isDestroying_ = false;

        bool needToQuit_ = true;

        WindowID thisID_ = WindowID::Main;
        MonitorID keyboardMonitorID_ = MonitorID::MainWindowKeyboard;
        MonitorID mouseMonitorID_ = MonitorID::MainWindowMouse;

        std::unique_ptr<ISceneUpdator> sceneUpdator_ = nullptr;
        std::unique_ptr<IDynamicContainer<IComponentRunner>> compRunnerCont_ = nullptr;

        std::unique_ptr<wb::IWindowFacade> &GetWindowFacade(const std::unique_ptr<ContainerStorage> &contStorage);

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
        /**************************************************************************************************************/

        WindowEvent() = default;
        ~WindowEvent() override = default;

        /***************************************************************************************************************
         * IEventの実装
        /**************************************************************************************************************/

        bool IsDestroying() override;

        /*******************************************************************************************************************
         * IWindowEventの実装
        /******************************************************************************************************************/

        void SetUpNeedToQuitFlag(bool needToQuitFlag) override;

        void SetUpWindowID(WindowID windowID) override;
        void SetUpKeyboardMonitorID(MonitorID monitorID) override;
        void SetUpMouseMonitorID(MonitorID monitorID) override;

        void SetUpSceneUpdator(std::unique_ptr<ISceneUpdator> sceneUpdator) override;
        void SetUpComponentRunnerCont(std::unique_ptr<IDynamicContainer<IComponentRunner>> compRunnerCont) override;
        bool IsSetUp() const override;

        void OnKeyDown(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) override;
        void OnKeyUp(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) override;
        void OnMouse(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) override;

        void OnSetFocus(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) override;
        void OnKillFocus(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) override;

        void OnSize(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) override;
        void OnMove(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) override;
        void OnPaint(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) override;

        void OnClose(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) override;
        void OnDestroy(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) override;
    };

} // namespace wb