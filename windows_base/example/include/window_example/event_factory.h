#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class ExampleWindowEventFactory : public wb::IWindowEventFactory
    {
    public:
        ExampleWindowEventFactory() = default;
        ~ExampleWindowEventFactory() override = default;

        std::unique_ptr<wb::IWindowEvent> Create() const override;
    };

} // namespace example

