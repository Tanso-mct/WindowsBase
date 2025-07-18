#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class ExampleEntitiesFactory : public wb::IEntitiesFactory
    {
    public:
        ExampleEntitiesFactory() = default;
        ~ExampleEntitiesFactory() override = default;

        void Create
        (
            wb::IAssetContainer &assetCont, 
            wb::IEntityContainer &entityCont, 
            wb::IComponentContainer &componentCont, 
            wb::IEntityIDView &entityIDView
        ) const override;
    };

} // namespace example