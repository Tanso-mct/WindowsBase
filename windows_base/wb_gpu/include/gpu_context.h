#pragma once

#include "wb_gpu/include/dll_config.h"

#include "wb_interface/gpu.h"

namespace wb
{
    class WB_GPU_API GpuContext : public IGpuContext
    {
    private:
        bool isCreated_ = false;

        Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Device4> d3d12Device_ = nullptr;
        D3D_FEATURE_LEVEL featureLevel_ = D3D_FEATURE_LEVEL_12_0;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;

        /***************************************************************************************************************
         * GpuContextクラスの実装
        /**************************************************************************************************************/

        // シングルトンインスタンス
        static GpuContext &Instance();
        
    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
        /**************************************************************************************************************/
        GpuContext() = default;
        virtual ~GpuContext() = default;

        /***************************************************************************************************************
         * IGpuContextインターフェースの実装
        /**************************************************************************************************************/

        bool &IsCreated() override;

        Microsoft::WRL::ComPtr<IDXGIFactory4> &DXGIFactory() override;
        Microsoft::WRL::ComPtr<ID3D12Device4> &D3D12Device() override;
        D3D_FEATURE_LEVEL &FeatureLevel() override;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> &CommandQueue() override;
        
    };

} // namespace wb