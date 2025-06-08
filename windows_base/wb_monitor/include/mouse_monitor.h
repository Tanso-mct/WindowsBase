#pragma once

#include "wb_monitor/include/dll_config.h"

#include "wb_interface/monitor.h"

namespace wb
{
    class WB_MONITOR_API MouseMonitor : public IMouseMonitor
    {
    private:
        std::unique_ptr<IMouseContext> mouseContext_;
        std::unique_ptr<IMouseCodeTable> mouseCodeTable_;
        std::unique_ptr<IInputTypeTable> inputTypeTable_;

    public:
        MouseMonitor() = default;
        ~MouseMonitor() override = default;

        /***************************************************************************************************************
         * IMonitorの実装
        /**************************************************************************************************************/

        MonitorType GetType() const override { return MonitorType::Mouse; }
        void UpdateState() override;

        /***************************************************************************************************************
         * IMouseMonitorの実装
        /**************************************************************************************************************/

        void SetUpContext(std::unique_ptr<IMouseContext> context) override;
        void SetUpCodeTable(std::unique_ptr<IMouseCodeTable> mouseCodeTable) override;
        void SetUpInputTypeTable(std::unique_ptr<IInputTypeTable> inputTypeTable) override;
        bool IsSetUp() const override;

        void EditState(UINT msg, WPARAM wParam, LPARAM lParam) override;

        bool GetButton(const MouseCode &mouseCode) override;
        bool GetButtonDown(const MouseCode &mouseCode) override;
        bool GetButtonUp(const MouseCode &mouseCode) override;
        bool GetButtonDoubleTap(const MouseCode &mouseCode, const double &threshold) override;

        const int &GetScrollDelta() override;
        const UINT &GetPositionX() override;
        const UINT &GetPositionY() override;
    };
} // namespace wb