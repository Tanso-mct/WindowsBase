#include "example/src/pch.h"
#include "example/include/scene_example/entities_factory.h"

#include "example/include/feature/prefab_example.h"

void example::ExampleEntitiesFactory::Create
(
    wb::IAssetContainer &assetCont, 
    wb::IEntityContainer &entityCont, 
    wb::IComponentContainer &componentCont, 
    wb::IEntityIDView &entityIDView
) const
{
    example::ExamplePrefab prefab;
    std::unique_ptr<wb::IOptionalValue> entityID = prefab.Create
    (
        assetCont, entityCont, componentCont, entityIDView
    );
}