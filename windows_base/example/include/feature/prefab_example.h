#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class ExamplePrefab: public wb::IPrefab
    {
    public:
        ExamplePrefab() = default;
        ~ExamplePrefab() override = default;

        std::unique_ptr<wb::IOptionalValue> Create
        (
            wb::IAssetContainer &assetCont,
            wb::IEntityContainer &entityCont,
            wb::IComponentContainer &componentCont,
            wb::IEntityIDView &entityIDView
        ) const override;

        std::vector<size_t> GetNeedAssetIDs() const override;
    };
} // namespace example