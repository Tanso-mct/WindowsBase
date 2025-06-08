#pragma once

#include "wb_window/include/dll_config.h"

#include "wb_interface/window.h"

namespace wb
{

    /*******************************************************************************************************************
     * Main Window
    /******************************************************************************************************************/

    class WB_WINDOW_API MainWindowFacadeFactory : public IWindowFacadeFactory
    {
    public:
        MainWindowFacadeFactory() = default;
        ~MainWindowFacadeFactory() override = default;

        /***************************************************************************************************************
         * IWindowFacadeFactoryインターフェースの実装
        /**************************************************************************************************************/

        std::unique_ptr<IWindowFacade> Create() override;
    };

    class WB_WINDOW_API MainWindowEventFactory : public IWindowEventFactory
    {
    public:
        MainWindowEventFactory() = default;
        ~MainWindowEventFactory() override = default;

        /***************************************************************************************************************
         * IWindowEventFactoryインターフェースの実装
        /**************************************************************************************************************/

        std::unique_ptr<IWindowEvent> Create() override;
    };

} // namespace wb