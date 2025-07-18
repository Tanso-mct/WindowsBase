#pragma once
#include "windows_base/windows_base.h"

#include "example/include/feature/interfaces/component_example.h"

namespace example
{
    const size_t &ExampleComponentID();

    class ExampleComponent : public IExampleComponent
    {
    public:
        ExampleComponent() = default;
        ~ExampleComponent() override = default;

        const size_t &GetID() const override;
    };
} // namespace example