#pragma once

#include "wb_monitor/include/dll_config.h"

#include "wb_interface/monitor.h"

#include "wb_utility/include/functor.h"

namespace wb
{
    class WB_MONITOR_API KeyCodeTable : public IKeyCodeTable
    {
    private:
        std::unordered_map<std::pair<WPARAM, bool>, KeyCode, PairHash, PairEqual> keyCodeMap_;

    public:
        KeyCodeTable() = default;
        ~KeyCodeTable() override = default;

        /***************************************************************************************************************
         * IKeyCodeTableの実装
        /**************************************************************************************************************/

        void SetUpTable() override;
        bool IsSetUp() const override;

        KeyCode GetKeyCode(WPARAM wParam, LPARAM lParam) const override;
    };
}