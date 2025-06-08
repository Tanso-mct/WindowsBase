#pragma once

#include "wb_monitor/include/dll_config.h"

#include "wb_interface/monitor.h"

#include "wb_utility/include/functor.h"

namespace wb
{
    constexpr UINT NOT_XBUTTON = 0;

    class WB_MONITOR_API MouseCodeTable : public IMouseCodeTable
    {
    private:
        std::unordered_map<std::pair<UINT, UINT>, MouseCode, PairHash, PairEqual> mouseCodeMap_;

    public:
        MouseCodeTable() = default;
        ~MouseCodeTable() override = default;

        /***************************************************************************************************************
         * IMouseCodeTableの実装
        /**************************************************************************************************************/

        void SetUpTable() override;
        bool IsSetUp() const override;

        MouseCode GetMouseCode(UINT msg, WPARAM wParam) const override;
    };
}