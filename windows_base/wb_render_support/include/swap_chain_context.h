#pragma once

#include "wb_render_support/include/dll_config.h"

#include "wb_interface/render_support.h"

namespace wb
{
    class WB_RENDER_SUPPORT_API SwapChainContext : public ISwapChainContext
    {
    private:
        Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain_ = nullptr;
        const UINT frameCount_;
        UINT frameIndex_ = 0;

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
        /**************************************************************************************************************/
        
        SwapChainContext(UINT frameCount);
        ~SwapChainContext() override = default;

        /***************************************************************************************************************
         * ISwapChainContextインタフェースの実装
        /**************************************************************************************************************/

        Microsoft::WRL::ComPtr<IDXGISwapChain3> &SwapChain() override { return swapChain_; }
        const UINT &FrameCount() const override { return frameCount_; }
        UINT &FrameIndex() override { return frameIndex_; } 
    };
}