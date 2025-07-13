#include "pch.h"

#include "windows_base/include/container_storage.h"
#include "windows_base/include/container_impl.h"
#pragma comment(lib, "windows_base.lib")

TEST(ContainerStorage, SetAndGetSceneContainer)
{
    wb::ContainerStorage storage;

    {
        std::unique_ptr<wb::ISceneContainer> sceneContainer = std::make_unique<wb::SceneContainer>();
        storage.SetContainer(std::move(sceneContainer));
    }
    
    {
        wb::ISceneContainer &sceneContainer = storage.GetContainer<wb::ISceneContainer>();
        EXPECT_NE(&sceneContainer, nullptr);
    }
}

TEST(ContainerStorage, SetAndGetAssetContainer)
{
    wb::ContainerStorage storage;

    {
        std::unique_ptr<wb::IAssetContainer> assetContainer = std::make_unique<wb::AssetContainer>();
        storage.SetContainer(std::move(assetContainer));
    }
    
    {
        wb::IAssetContainer &assetContainer = storage.GetContainer<wb::IAssetContainer>();
        EXPECT_NE(&assetContainer, nullptr);
    }
}