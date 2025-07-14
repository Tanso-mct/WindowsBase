#include "pch.h"

#include "windows_base/include/scene.h"
#include "windows_base/include/entity.h"
#include "windows_base/include/container_impl.h"
#include "windows_base/include/asset_group.h"
#include "windows_base/include/id_factory.h"
#include "windows_base/include/file_loader_collection.h"
#include "windows_base/include/asset_factory_collection.h"
#include "windows_base/include/asset_collection.h"
#include "windows_base/include/container_storage.h"
#include "windows_base/include/system.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    /*******************************************************************************************************************
     * Entities Factory Mock
     * Entities factory is responsible for creating entities in the scene.
     * Create an instance of entity and store it in the entity container.
    /******************************************************************************************************************/

    class MockEntitiesFactory : public wb::IEntitiesFactory
    {
    public:
        void Create
        (
            wb::IAssetContainer &assetCont, wb::IEntityContainer &entityCont,
            wb::IComponentContainer &componentCont, wb::IEntityIDView &entityIDView
        ) const override
        {
            // Mock implementation
        }
    };

    const size_t &MockAssetFactoryID()
    {
        static size_t id = wb::IDFactory::CreateAssetFactoryID();
        return id;
    }

    class MockAssetFactory : public wb::IAssetFactory
    {
    public:
        MockAssetFactory() = default;
        virtual ~MockAssetFactory() override = default;

        std::unique_ptr<wb::IAsset> Create(const wb::IFileData &fileData) const override
        {
            // Mock implementation for testing purposes
            return nullptr;
        }
    };

    WB_REGISTER_ASSET_FACTORY(MockAssetFactoryID(), MockAssetFactory);

    const size_t &MockFileLoaderID()
    {
        static size_t id = wb::IDFactory::CreateFileLoaderID();
        return id;
    }

    class MockFileLoader : public wb::IFileLoader
    {
    public:
        MockFileLoader() = default;
        ~MockFileLoader() override = default;

        const size_t &GetID() const override
        {
            return MockFileLoaderID();
        }

        std::unique_ptr<wb::IFileData> Load(std::string_view path) override
        {
            // Mock implementation, returning nullptr for simplicity
            return nullptr;
        }
    };

    WB_REGISTER_FILE_LOADER(MockFileLoaderID(), MockFileLoader);

    const size_t &MockAssetID()
    {
        static size_t id = wb::IDFactory::CreateAssetID();
        return id;
    }

    constexpr const char* MOCK_FILE_PATH = "mock_file_path";
    WB_REGISTER_ASSET(MockAssetID(), MockAssetFactoryID(), MockFileLoaderID(), MOCK_FILE_PATH);

    class MockAssetGroup : public wb::AssetGroup
    {
    public:
        MockAssetGroup()
        {
            AddAssetID(MockAssetID());
        }
    };

    class MockSystemScheduler : public wb::ISystemScheduler
    {
    public:
        void Execute(const wb::SystemArgument &args) override
        {
            // Mock implementation
        }
    };
}

TEST(SceneFacade, Create)
{
    // Create a SceneFacade
    wb::SceneFacadeFactory factory;
    std::unique_ptr<wb::ISceneFacade> sceneFacade = factory.Create
    (
        std::make_unique<wb::SceneContext>(),
        std::make_unique<MockEntitiesFactory>(),
        std::make_unique<wb::EntityIDViewFactory<wb::EntityIDView>>(),
        std::make_unique<wb::SystemsFactory>(),
        std::make_unique<MockAssetGroup>(),
        std::make_unique<MockSystemScheduler>()
    );

    // Check if the scene facade is created successfully
    EXPECT_TRUE(sceneFacade->CheckIsReady());
}

TEST(SceneFacade, Use)
{
    // Create a SceneFacade
    wb::SceneFacadeFactory factory;
    std::unique_ptr<wb::ISceneFacade> sceneFacade = factory.Create
    (
        std::make_unique<wb::SceneContext>(),
        std::make_unique<MockEntitiesFactory>(),
        std::make_unique<wb::EntityIDViewFactory<wb::EntityIDView>>(),
        std::make_unique<wb::SystemsFactory>(),
        std::make_unique<MockAssetGroup>(),
        std::make_unique<MockSystemScheduler>()
    );

    // Create an asset container
    std::unique_ptr<wb::IAssetContainer> assetCont = std::make_unique<wb::AssetContainer>();
    assetCont->Create(wb::gAssetCollection.GetMaxID() + 1);

    // Load the scene
    sceneFacade->Load(*assetCont);

    // Update the scene
    wb::ContainerStorage contStorage;
    size_t nextSceneID = 0;
    const double MOCK_DELTA_TIME = 0.016; // 16 ms for 60 FPS
    const size_t MOCK_BELONG_WINDOW_ID = 1;
    sceneFacade->Update(contStorage, MOCK_DELTA_TIME, MOCK_BELONG_WINDOW_ID, nextSceneID);

    // Release the scene
    sceneFacade->Release(*assetCont);
}