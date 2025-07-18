#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class IExampleComponent : public wb::IComponent
    {
    public:
        virtual ~IExampleComponent() = default;
    };
} // namespace example