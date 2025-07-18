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

        virtual const size_t &GetDataAssetID() const = 0;
        virtual void SetDataAssetID(size_t dataAssetID) = 0;
    };
} // namespace example