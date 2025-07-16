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

TEST(ContainerStorage, SetAndGetMonitorContainer)
{
    wb::ContainerStorage storage;

    {
        std::unique_ptr<wb::IMonitorContainer> monitorContainer = std::make_unique<wb::MonitorContainer>();
        storage.SetContainer(std::move(monitorContainer));
    }
    
    {
        wb::IMonitorContainer &monitorContainer = storage.GetContainer<wb::IMonitorContainer>();
        EXPECT_NE(&monitorContainer, nullptr);
    }
}

TEST(ContainerStorage, SetAndGetWindowContainer)
{
    wb::ContainerStorage storage;

    {
        std::unique_ptr<wb::IWindowContainer> windowContainer = std::make_unique<wb::WindowContainer>();
        storage.SetContainer(std::move(windowContainer));
    }
    
    {
        wb::IWindowContainer &windowContainer = storage.GetContainer<wb::IWindowContainer>();
        EXPECT_NE(&windowContainer, nullptr);
    }
}

TEST(ContainerStorage, SetAndGetSharedContainer)
{
    wb::ContainerStorage storage;

    {
        std::unique_ptr<wb::ISharedContainer> sharedContainer = std::make_unique<wb::SharedContainer>();
        storage.SetContainer(std::move(sharedContainer));
    }
    
    {
        wb::ISharedContainer &sharedContainer = storage.GetContainer<wb::ISharedContainer>();
        EXPECT_NE(&sharedContainer, nullptr);
    }
}

