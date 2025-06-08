#include "pch.h"

#include "wb_container/include/container_storage.h"

TEST(ContainerStorage, GetContainer)
{
    std::unique_ptr<wb::ContainerStorage> contStorage = std::make_unique<wb::ContainerStorage>();

    {
        std::unique_ptr<wb::IWindowContainer>& windowCont = contStorage->Get<wb::IWindowContainer>();
        EXPECT_EQ(windowCont, nullptr);
    }

    {
        std::unique_ptr<wb::ISceneContainer>& sceneCont = contStorage->Get<wb::ISceneContainer>();
        EXPECT_EQ(sceneCont, nullptr);
    }

    {
        std::unique_ptr<wb::ICommonContainer>& commonCont = contStorage->Get<wb::ICommonContainer>();
        EXPECT_EQ(commonCont, nullptr);
    }

    {
        std::unique_ptr<wb::IMonitorContainer>& monitorCont = contStorage->Get<wb::IMonitorContainer>();
        EXPECT_EQ(monitorCont, nullptr);
    }

    {
        std::unique_ptr<wb::IAssetContainer>& assetCont = contStorage->Get<wb::IAssetContainer>();
        EXPECT_EQ(assetCont, nullptr);
    }
}

TEST(ContainerStorage, GetInvalidContainer)
{
    std::unique_ptr<wb::ContainerStorage> contStorage = std::make_unique<wb::ContainerStorage>();
    // std::unique_ptr<int>& invalidCont = contStorage->Get<int>(); // コンパイルエラー
}