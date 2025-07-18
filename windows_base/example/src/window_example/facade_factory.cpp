#include "example/src/pch.h"
#include "example/include/window_example/facade_factory.h"

#include "example/include/window_example/monitor_keyboard.h"
#include "example/include/window_example/monitor_mouse.h"

std::unique_ptr<wb::IWindowFacade> example::ExampleWindowFacadeFactory::Create() const
{
    std::unique_ptr<wb::IWindowFacade> facade = std::make_unique<wb::DefaultWindowFacade>();

    {
        std::unique_ptr<wb::IWindowContext> context = std::make_unique<wb::WindowContext>();
        context->Name() = L"Example Window";

        facade->SetContext(std::move(context));
    }

    facade->AddMonitorID(example::ExampleKeyboardMonitorID());
    facade->AddMonitorID(example::ExampleMouseMonitorID());

    return facade;
}