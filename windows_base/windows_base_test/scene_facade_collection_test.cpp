#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/scene_facade_collection.h"
#include "windows_base/include/container_impl.h"
#include "windows_base/include/asset_group.h"
#include "windows_base/include/file_loader_collection.h"
#include "windows_base/include/asset_factory_collection.h"
#include "windows_base/include/asset_collection.h"
#include "windows_base/include/container_storage.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    /*******************************************************************************************************************
     * Mock Asset Factory
    /******************************************************************************************************************/

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

    /*******************************************************************************************************************
     * Mock File Loader
    /******************************************************************************************************************/

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

    /*******************************************************************************************************************
     * Mock Asset
    /******************************************************************************************************************/

    const size_t &MockAssetID()
    {
        static size_t id = wb::IDFactory::CreateAssetID();
        return id;
    }

    constexpr const char* MOCK_FILE_PATH = "mock_file_path";
    WB_REGISTER_ASSET(MockAssetID, MockAssetFactoryID(), MockFileLoaderID(), MOCK_FILE_PATH);

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

    /*******************************************************************************************************************
     * Mock Asset Group
     * Contains asset IDs that will be used in the scene.
    /******************************************************************************************************************/

    class MockAssetGroup : public wb::AssetGroup
    {
    public:
        MockAssetGroup()
        {
            AddAssetID(MockAssetID());
        }
    };

    /*******************************************************************************************************************
     * Mock System Scheduler
     * Responsible for updating systems in the scene.
    /******************************************************************************************************************/

    class MockSystemScheduler : public wb::ISystemScheduler
    {
    public:
        void Execute(wb::ISystemContainer &systemCont, wb::SystemArgument &args) override
        {
            // Mock implementation
        }
    };

    /*******************************************************************************************************************
     * Mock Scene Facade Factory
     * Creates a SceneFacade with the necessary factories and asset group.
    /******************************************************************************************************************/

    const size_t &MockSceneFacadeID()
    {
        static size_t id = wb::IDFactory::CreateSceneFacadeID();
        return id;
    }

    WB_REGISTER_SCENE_FACADE
    (
        MockSceneFacadeID,
        MockEntitiesFactory,
        MockAssetGroup,
        MockSystemScheduler
    );
    
}

TEST(SceneFacadeCollection, GetFactory)
{
    wb::ISceneFacadeFactory &factory = wb::gSceneFacadeCollection.GetFactory(MockSceneFacadeID());
    EXPECT_NE(&factory, nullptr);

    std::unique_ptr<wb::ISceneFacade> sceneFacade = factory.Create();
    EXPECT_NE(sceneFacade, nullptr);
    EXPECT_TRUE(sceneFacade->CheckIsReady());
}

