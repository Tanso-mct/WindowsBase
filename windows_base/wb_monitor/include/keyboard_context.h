#pragma once

#include "wb_monitor/include/dll_config.h"

#include "wb_interface/monitor.h"

namespace wb
{
    class WB_MONITOR_API KeyboardContext : public IKeyboardContext
    {
    private:
        std::vector<UINT> keyStates_;
        KeyCode previousKey_;
        std::chrono::high_resolution_clock::time_point previousKeyTime_;

    public:
        KeyboardContext() = default;
        ~KeyboardContext() override = default;

        /***************************************************************************************************************
         * IKeyboardContextの実装
        /**************************************************************************************************************/

        std::vector<UINT> &KeyStates() override { return keyStates_; }
        KeyCode &PreviousKey() override { return previousKey_; }
        std::chrono::high_resolution_clock::time_point &PreviousKeyTime() override { return previousKeyTime_; }
    };

}