#pragma once

#include "wb_window/include/dll_config.h"

#include "wb_interface/window.h"
#include "wb_interface/render_support.h"

#include <memory>

namespace wb
{
    constexpr const wchar_t* TASK_BAR_CLASS_NAME = L"Shell_TrayWnd";

    class WB_WINDOW_API WindowFacade : public IWindowFacade
    {
    private:
        const UINT RENDER_TARGET_COUNT;
        const UINT DEPTH_STENCIL_COUNT;

        std::unique_ptr<IWindowContext> windowContext_ = nullptr;
        std::unique_ptr<ISwapChainContext> swapChainContext_ = nullptr;
        std::unique_ptr<IRenderTargetContext> renderTargetContext_ = nullptr;
        std::unique_ptr<IFenceContext> fenceContext_ = nullptr;

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
         **************************************************************************************************************/

        WindowFacade(UINT renderTargetCount, UINT depthStencilCount);
        virtual ~WindowFacade() = default;

        /***************************************************************************************************************
         * IWindowFacadeインターフェースの実装
         **************************************************************************************************************/

        void SetUpWindowContext(std::unique_ptr<IWindowContext> windowContext) override;
        void SetUpSwapChainContext(std::unique_ptr<ISwapChainContext> swapChainContext) override;
        void SetUpRenderTargetContext(std::unique_ptr<IRenderTargetContext> renderTargetContext) override;
        void SetUpFenceContext(std::unique_ptr<IFenceContext> fenceContext) override;
        bool IsSetUp() const override;

        const HWND &GetHandle() const override;
        std::wstring_view GetName() const override;

        void Create(WNDCLASSEX& wc) override;
        bool IsCreated() const override;

        void Destroy() override;
        void Destroyed() override;

        void Resize(UINT width, UINT height) override;;
        void Resized() override;

        bool IsFocusing() const override;
        void Focus() override;
        void Focused() override;
        void UnFocused() override;

        bool IsMaximizing() const override;;
        void Maximize() override;
        void Maximized() override;

        bool IsMinimizing() const override;;
        void Minimize() override;
        void Minimized() override;

        bool IsFullScreen() const override;
        void FullScreen() override;
        void FullScreened() override;

        void Restore() override;
        void Restored() override;

        void Move(UINT posX, UINT posY) override;
        void Moved() override;

        void SetBarrierToRenderTarget(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList) override;
        void SetBarrierToPresent(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList) override;
        void SetRenderTarget(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList) override;

        void ClearViews
        (
            const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &cmdList,
            const float (&clearColor)[4]
        ) override;
        
        void Present() override;

        void WaitForGpu() override;
        void WaitThisFrame() override;
    };

} // namespace wb