#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class ExampleSystemScheduler : public wb::ISystemScheduler
    {
    public:
        ExampleSystemScheduler() = default;
        ~ExampleSystemScheduler() override = default;

        void Execute(wb::ISystemContainer &systemCont, wb::SystemArgument &args) override;
    };

} // namespace example