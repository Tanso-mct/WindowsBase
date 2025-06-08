#pragma once

#include "wb_monitor/include/dll_config.h"

#include "wb_interface/monitor.h"

#include "wb_utility/include/functor.h"

namespace wb
{
    class WB_MONITOR_API MouseInputTypeTable : public IInputTypeTable
    {
    private:
        std::unordered_map<UINT, UINT> inputTypeMap_;

    public:
        MouseInputTypeTable() = default;
        ~MouseInputTypeTable() override = default;

        /***************************************************************************************************************
         * IInputTypeTableの実装
        /**************************************************************************************************************/

        void SetUpTable() override;
        bool IsSetUp() const override;

        UINT GetInputType(UINT msg) const override;
    };

} // namespace wb