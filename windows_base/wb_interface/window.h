#pragma once

#include "wb_interface/event.h"
#include "wb_interface/container.h"
#include "wb_interface/data_config.h"

#include <windows.h>
#include <string>
#include <string_view>
#include <d3d12.h>
#include <wrl/client.h>
#include <memory>

namespace wb
{
    class IWindowContext
    {
    public:
        virtual ~IWindowContext() = default;

        virtual HWND &Handle() = 0;
        virtual HINSTANCE &Instance() = 0;
        virtual HWND &ParentHandle() = 0;

        virtual DWORD &Style() = 0;
        virtual std::wstring &Name() = 0;
        virtual UINT &PosX() = 0;
        virtual UINT &PosY() = 0;
        virtual UINT &Width() = 0;
        virtual UINT &Height() = 0;
        virtual UINT &ClientWidth() = 0;
        virtual UINT &ClientHeight() = 0;

        virtual bool &IsCreated() = 0;
        virtual bool &IsFocus() = 0;
        virtual bool &IsMaximized() = 0;
        virtual bool &IsMinimized() = 0;
        virtual bool &IsFullScreen() = 0;
    };

    constexpr UINT DEFAULT_WINDOW_WIDTH = 960;
    constexpr UINT DEFAULT_WINDOW_HEIGHT = 540;

    class ISwapChainContext;
    class IRenderTargetContext;
    class IFenceContext;

    class IWindowFacade
    {
    public:
        virtual ~IWindowFacade() = default;

        /***************************************************************************************************************
         * 各種セットアップメソッド
         * WindowFacadeクラスのメソッドを使う前に行う必要がある
         * インタフェースを渡すことで、依存度を下げる
        /**************************************************************************************************************/

        virtual void SetUpWindowContext(std::unique_ptr<IWindowContext> windowContext) = 0;
        virtual void SetUpSwapChainContext(std::unique_ptr<ISwapChainContext> swapChainContext) = 0;
        virtual void SetUpRenderTargetContext(std::unique_ptr<IRenderTargetContext> renderTargetContext) = 0;
        virtual void SetUpFenceContext(std::unique_ptr<IFenceContext> fenceContext) = 0;

        // セットアップが完了したかを確認するメソッド
        virtual bool IsSetUp() const = 0;

        /*******************************************************************************************************************
         * Windowの情報を取得するメソッド
        /******************************************************************************************************************/

        virtual const HWND &GetHandle() const = 0;
        virtual std::wstring_view GetName() const = 0;

        /***************************************************************************************************************
         * 各ウィンドウへの処理と、その処理が実際に行われた後に行う必要がある処理ら
         * 現在形のメソッドは、ウィンドウの状態を変更するためのメソッド
         * 過去形のメソッドは、ウィンドウの状態が変更された後に呼び出されるメソッド
        /**************************************************************************************************************/

        virtual void Create(WNDCLASSEX& wc) = 0;
        virtual bool IsCreated() const = 0;

        virtual void Destroy() = 0;
        virtual void Destroyed() = 0;

        virtual void Resize(UINT width, UINT height) = 0;
        virtual void Resized() = 0;

        virtual bool IsFocusing() const = 0;
        virtual void Focus() = 0;
        virtual void Focused() = 0;
        virtual void UnFocused() = 0;

        virtual bool IsMaximizing() const = 0;
        virtual void Maximize() = 0;
        virtual void Maximized() = 0;

        virtual bool IsMinimizing() const = 0;
        virtual void Minimize() = 0;
        virtual void Minimized() = 0;

        virtual bool IsFullScreen() const = 0;
        virtual void FullScreen() = 0;
        virtual void FullScreened() = 0;

        virtual void Restore() = 0;
        virtual void Restored() = 0;

        virtual void Move(UINT posX, UINT posY) = 0;
        virtual void Moved() = 0;

        /***************************************************************************************************************
         * レンダリングに関するメソッド
        /**************************************************************************************************************/

        virtual void SetBarrierToRenderTarget(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList) = 0;
        virtual void SetBarrierToPresent(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList) = 0;
        virtual void SetRenderTarget(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList) = 0;

        virtual void ClearViews
        (
            const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
            const float (&clearColor)[4]
        ) = 0;

        virtual void Present() = 0;

        virtual void WaitForGpu() = 0;

        // フレームインデックスの更新は、現在の描画が完了した後行うため、このメソッドで行う
        virtual void WaitThisFrame() = 0;
    };

    class ContainerStorage;
    class ISceneUpdator;
    class IComponentRunner;

    class IWindowEvent : public IEvent
    {
    public:
        virtual ~IWindowEvent() = default;

        /***************************************************************************************************************
         * 各種セットアップメソッド
         * WindowEventクラスのメソッドを使う前に行う必要がある
        /**************************************************************************************************************/

        virtual void SetUpNeedToQuitFlag(bool needToQuitFlag) = 0;

        virtual void SetUpWindowID(WindowID windowID) = 0;
        virtual void SetUpKeyboardMonitorID(MonitorID monitorID) = 0;
        virtual void SetUpMouseMonitorID(MonitorID monitorID) = 0;
        
        virtual void SetUpSceneUpdator(std::unique_ptr<ISceneUpdator> sceneUpdator) = 0;
        virtual void SetUpComponentRunnerCont(std::unique_ptr<IDynamicContainer<IComponentRunner>> compRunnerCont) = 0;

        // セットアップが完了したかを確認するメソッド
        virtual bool IsSetUp() const = 0;

        /***************************************************************************************************************
         * 各種イベントハンドラメソッド
         * これらのメソッドは、Windowsのメッセージループで呼び出されることを想定している
        /**************************************************************************************************************/

        virtual void OnKeyDown(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
        virtual void OnKeyUp(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
        virtual void OnMouse(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

        virtual void OnSetFocus(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
        virtual void OnKillFocus(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

        virtual void OnSize(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
        virtual void OnMove(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
        virtual void OnPaint(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

        virtual void OnClose(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
        virtual void OnDestroy(const std::unique_ptr<ContainerStorage> &contStorage, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
    };

    class IWindowFacadeFactory
    {
    public:
        virtual ~IWindowFacadeFactory() = default;
        virtual std::unique_ptr<IWindowFacade> Create() = 0;
    };

    class IWindowEventFactory
    {
    public:
        virtual ~IWindowEventFactory() = default;
        virtual std::unique_ptr<IWindowEvent> Create() = 0;
    };

} // namespace wb