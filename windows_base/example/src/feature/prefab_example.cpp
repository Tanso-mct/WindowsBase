#include "example/src/pch.h"
#include "example/include/feature/prefab_example.h"

#include "example/include/feature/component_example.h"
#include "example/include/feature/asset.h"

std::unique_ptr<wb::IOptionalValue> example::ExamplePrefab::Create
(
    wb::IAssetContainer &assetCont, 
    wb::IEntityContainer &entityCont, 
    wb::IComponentContainer &componentCont, 
    wb::IEntityIDView &entityIDView
) const
{
    // Create an entity
    wb::CreatingEntity entity = wb::CreateEntity(entityCont, entityIDView);

    // Add component to the entity
    entity().AddComponent(ExampleComponentID(), componentCont);

    // Initialize the component
    wb::IComponent *component = entity().GetComponent(ExampleComponentID(), componentCont);
    example::IExampleComponent *example = wb::As<IExampleComponent>(component);
    example->SetDataAssetID(example::ExampleDataAssetID());

    // Return the entity ID
    return entity().GetID().Clone();
}

std::vector<size_t> example::ExamplePrefab::GetNeedAssetIDs() const
{
    std::vector<size_t> assetIDs;
    assetIDs.emplace_back(example::ExampleDataAssetID());

    return assetIDs;
}