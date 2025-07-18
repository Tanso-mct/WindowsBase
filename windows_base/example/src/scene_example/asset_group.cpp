#include "example/src/pch.h"
#include "example/include/scene_example/asset_group.h"

#include "example/include/feature/prefab_example.h"

example::ExampleAssetGroup::ExampleAssetGroup()
{
    example::ExamplePrefab prefab;

    // Add the asset IDs needed by the prefab to the asset group
    for (const size_t &assetID : prefab.GetNeedAssetIDs())
    {
        AddAssetID(assetID);
    }
}