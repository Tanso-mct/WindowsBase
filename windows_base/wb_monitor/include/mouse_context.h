#pragma once

#include "wb_monitor/include/dll_config.h"

#include "wb_interface/monitor.h"

namespace wb
{
    class WB_MONITOR_API MouseContext : public IMouseContext
    {
    private:
        std::vector<UINT> buttonStates_;
        MouseCode previousButton_ = MouseCode::Null;
        std::chrono::high_resolution_clock::time_point previousButtonTime_;

        int scrollDelta_ = 0;
        UINT positionX_ = 0;
        UINT positionY_ = 0;

    public:
        MouseContext() = default;
        ~MouseContext() override = default;

        /***************************************************************************************************************
         * IMouseContextの実装
        /**************************************************************************************************************/

        std::vector<UINT> &ButtonStates() override { return buttonStates_; }
        MouseCode &PreviousButton() override { return previousButton_; }
        std::chrono::high_resolution_clock::time_point &PreviousButtonTime() override { return previousButtonTime_; }

        int &ScrollDelta() override { return scrollDelta_; }
        UINT &PositionX() override { return positionX_; }
        UINT &PositionY() override { return positionY_; }
    };

} // namespace wb