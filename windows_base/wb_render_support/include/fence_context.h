#pragma once

#include "wb_render_support/include/dll_config.h"

#include "wb_interface/render_support.h"

namespace wb
{
    constexpr UINT DEFAULT_FENCE_VALUE = 0;

    class WB_RENDER_SUPPORT_API FenceContext : public IFenceContext
    {
    private:
        Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
        std::vector<UINT64> fenceValues_;
        HANDLE fenceEvent_ = nullptr;

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
         **************************************************************************************************************/
        FenceContext(UINT frameCount);
        ~FenceContext() override;

        /***************************************************************************************************************
         * IFenceContextインタフェースの実装
         **************************************************************************************************************/
        Microsoft::WRL::ComPtr<ID3D12Fence> &Fence() override { return fence_; }
        std::vector<UINT64> &FenceValues() override { return fenceValues_; }
        HANDLE &FenceEvent() override { return fenceEvent_; }
    };
}