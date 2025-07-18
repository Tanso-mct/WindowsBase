#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class IDataAsset : public wb::IAsset
    {
    public:
        virtual ~IDataAsset() = default;

        virtual std::string_view GetData() const = 0;
        virtual void SetData(const std::string &data) = 0;
    };
}