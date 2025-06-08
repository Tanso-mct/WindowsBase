#pragma once

#include "wb_gpu/include/dll_config.h"

#include "wb_interface/gpu.h"

#include <memory>

namespace wb
{
    class WB_GPU_API GpuFacade : public IGpuFacade
    {
    private:
        std::unique_ptr<IGpuContext> gpuContext_ = nullptr;

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
         * unique_ptrを使用しているため、コピーコンストラクタとコピー代入演算子は削除
         **************************************************************************************************************/

        GpuFacade();
        virtual ~GpuFacade() = default;

        GpuFacade(const GpuFacade &) = delete;
        GpuFacade &operator=(const GpuFacade &) = delete;

        /***************************************************************************************************************
         * IGpuFacadeインターフェースの実装
         **************************************************************************************************************/

        bool IsCreated() override { return gpuContext_->IsCreated();}
        void Create() override;
        std::unique_ptr<IGpuContext> &GpuContext() override { return gpuContext_; }
    };

} // namespace wb