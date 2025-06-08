#pragma once

#include "wb_window/include/dll_config.h"

#include "wb_interface/window.h"

#include <unordered_map>
#include <memory>

namespace wb
{
    class WB_WINDOW_API WindowFacadeCollection
    {
    private:
        std::unordered_map<WindowID, std::unique_ptr<IWindowFacadeFactory>> windowFacadeFactories_;
        std::unique_ptr<IWindowFacadeFactory> empty_ = nullptr;

    public:
        WindowFacadeCollection();
        ~WindowFacadeCollection() = default;

        std::unique_ptr<IWindowFacadeFactory> &GetFactory(WindowID windowID);
    };

    class WB_WINDOW_API WindowEventCollection
    {
    private:
        std::unordered_map<WindowID, std::unique_ptr<IWindowEventFactory>> windowEventFactories_;
        std::unique_ptr<IWindowEventFactory> empty_ = nullptr;

    public:
        WindowEventCollection();
        ~WindowEventCollection() = default;

        std::unique_ptr<IWindowEventFactory> &GetFactory(WindowID windowID);
    };

} // namespace wb