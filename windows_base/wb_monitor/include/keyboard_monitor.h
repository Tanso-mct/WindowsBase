#pragma once

#include "wb_monitor/include/dll_config.h"

#include "wb_interface/monitor.h"

namespace wb
{

    class WB_MONITOR_API KeyboardMonitor : public IKeyboardMonitor
    {
    private:
        std::unique_ptr<IKeyboardContext> keyboardContext_ = nullptr;
        std::unique_ptr<IKeyCodeTable> keyCodeTable_ = nullptr;

    public:
        KeyboardMonitor() = default;
        ~KeyboardMonitor() override = default;

        /***************************************************************************************************************
         * IMonitorインタフェースの実装
        /**************************************************************************************************************/

        MonitorType GetType() const override { return MonitorType::KeyBoard; }
        void UpdateState() override;

        /***************************************************************************************************************
         * IKeyboardMonitorインタフェースの実装
        /**************************************************************************************************************/

        void SetUpContext(std::unique_ptr<IKeyboardContext> keyContext) override;
        void SetUpCodeTable(std::unique_ptr<IKeyCodeTable> keyCodeTable) override;
        bool IsSetUp() const override;

        void EditStateByDown(WPARAM wParam, LPARAM lParam) override;
        void EditStateByUp(WPARAM wParam, LPARAM lParam) override;
        
        bool GetKey(const KeyCode &keyCode) override;
        bool GetKeyDown(const KeyCode &keyCode) override;
        bool GetKeyUp(const KeyCode &keyCode) override;
        bool GetKeyDoubleTap(const KeyCode &keyCode, const double &threshold) override;
    };
}