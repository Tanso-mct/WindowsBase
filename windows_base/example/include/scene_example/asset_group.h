#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class ExampleAssetGroup : public wb::AssetGroup
    {
    public:
        ExampleAssetGroup();
        ~ExampleAssetGroup() override = default;
    };

} // namespace example