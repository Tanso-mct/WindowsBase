#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class ExampleWindowFacadeFactory : public wb::IWindowFacadeFactory
    {
    public:
        ExampleWindowFacadeFactory() = default;
        ~ExampleWindowFacadeFactory() override = default;

        std::unique_ptr<wb::IWindowFacade> Create() const override;
    };

} // namespace example