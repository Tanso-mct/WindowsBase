#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class IExampleComponent : public wb::IComponent
    {
    public:
        virtual ~IExampleComponent() = default;

        virtual const float &GetValue() const = 0;
        virtual void SetValue(float value) = 0;
    };
} // namespace example