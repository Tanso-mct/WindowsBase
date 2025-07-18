#include "pch.h"

#include "windows_base/include/scene.h"
#include "windows_base/include/entity.h"
#include "windows_base/include/container_impl.h"
#include "windows_base/include/asset_group.h"
#include "windows_base/include/id_factory.h"
#include "windows_base/include/file_loader_registry.h"
#include "windows_base/include/asset_factory_registry.h"
#include "windows_base/include/asset_registry.h"
#include "windows_base/include/container_storage.h"
#include "windows_base/include/system.h"
#include "windows_base/include/scene_facade_registry.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
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

            std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate loading delay

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
    WB_REGISTER_ASSET(MockAssetID, MockAssetFactoryID(), MockFileLoaderID(), MOCK_FILE_PATH);

    /*******************************************************************************************************************
     * Mock Scenes ID
    /******************************************************************************************************************/

    const size_t &MockSceneFacadeID()
    {
        static size_t id = wb::IDFactory::CreateSceneFacadeID();
        return id;
    }

    const size_t &MockNextScneFacadeID()
    {
        static size_t id = wb::IDFactory::CreateSceneFacadeID();
        return id;
    }

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
            std::cout << "MockSystemScheduler executed." << std::endl;

            args.state_ = wb::SceneState::Switching;
            args.nextSceneID_ = MockNextScneFacadeID();
        }
    };

    class MockOtherSystemScheduler : public wb::ISystemScheduler
    {
    public:
        void Execute(wb::ISystemContainer &systemCont, wb::SystemArgument &args) override
        {
            // Mock implementation for a different system scheduler
            std::cout << "MockOtherSystemScheduler executed." << std::endl;

            args.state_ = wb::SceneState::NeedToExit;
        }
    };

    /*******************************************************************************************************************
     * Mock Scenes registrar
     * SceneFacade with the necessary factories and asset group.
    /******************************************************************************************************************/

    WB_REGISTER_SCENE_FACADE
    (
        MockSceneFacadeID,
        MockEntitiesFactory,
        MockAssetGroup,
        MockSystemScheduler
    );


    WB_REGISTER_SCENE_FACADE
    (
        MockNextScneFacadeID,
        MockEntitiesFactory,
        MockAssetGroup,
        MockOtherSystemScheduler
    );
}

TEST(SceneFacade, Create)
{
    // Get the SceneFacade factory
    wb::ISceneFacadeFactory &factory = wb::gSceneFacadeRegistry.GetFactory(MockSceneFacadeID());

    // Create a SceneFacade using the factory
    std::unique_ptr<wb::ISceneFacade> sceneFacade = factory.Create();
}

TEST(SceneFacade, Use)
{
    // Get the SceneFacade factory
    wb::ISceneFacadeFactory &factory = wb::gSceneFacadeRegistry.GetFactory(MockSceneFacadeID());

    // Create a SceneFacade using the factory
    std::unique_ptr<wb::ISceneFacade> sceneFacade = factory.Create();

    // Create an asset container
    std::unique_ptr<wb::IAssetContainer> assetCont = std::make_unique<wb::AssetContainer>();
    assetCont->Create(wb::gAssetRegistry.GetMaxID() + 1);

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

TEST(SceneUpdator, Create)
{
    std::unique_ptr<wb::ISceneUpdator> sceneUpdator = std::make_unique<wb::SceneUpdator>();

    const size_t MOCK_BELONG_WINDOW_ID = 1;

    // Set the belong window ID and initial scene ID
    sceneUpdator->SetBelongWindowID(MOCK_BELONG_WINDOW_ID);
    sceneUpdator->SetInitialSceneID(MockSceneFacadeID());

    // Check if the scene updator is ready
    EXPECT_TRUE(sceneUpdator->CheckIsReady());
}

TEST(SceneUpdator, Use)
{
    std::unique_ptr<wb::ISceneUpdator> sceneUpdator = std::make_unique<wb::SceneUpdator>();

    const size_t MOCK_BELONG_WINDOW_ID = 1;

    // Set the belong window ID and initial scene ID
    sceneUpdator->SetBelongWindowID(MOCK_BELONG_WINDOW_ID);
    sceneUpdator->SetInitialSceneID(MockSceneFacadeID());

    // Check if the scene updator is ready
    EXPECT_TRUE(sceneUpdator->CheckIsReady());

    // Create containers
    wb::ContainerStorage contStorage;
    {
        std::unique_ptr<wb::IAssetContainer> assetCont = std::make_unique<wb::AssetContainer>();
        assetCont->Create(wb::gAssetRegistry.GetMaxID() + 1);
        contStorage.SetContainer<wb::IAssetContainer>(std::move(assetCont));

        std::unique_ptr<wb::ISceneContainer> sceneCont = std::make_unique<wb::SceneContainer>();
        sceneCont->Create(wb::gSceneFacadeRegistry.GetMaxID() + 1);
        contStorage.SetContainer<wb::ISceneContainer>(std::move(sceneCont));
    }

    // Create current scene
    {
        // Get the SceneFacade factory
        wb::ISceneFacadeFactory &factory = wb::gSceneFacadeRegistry.GetFactory(MockSceneFacadeID());

        // Create a SceneFacade using the factory
        std::unique_ptr<wb::ISceneFacade> sceneFacade = factory.Create();

        // Get the scene container from the container storage
        wb::ISceneContainer &sceneCont = contStorage.GetContainer<wb::ISceneContainer>();

        // Store the scene facade in the scene container
        sceneCont.Set(MockSceneFacadeID(), std::move(sceneFacade));
    }

    // Create next scene
    {
        // Get the SceneFacade factory
        wb::ISceneFacadeFactory &factory = wb::gSceneFacadeRegistry.GetFactory(MockNextScneFacadeID());

        // Create a SceneFacade using the factory
        std::unique_ptr<wb::ISceneFacade> sceneFacade = factory.Create();

        // Get the scene container from the container storage
        wb::ISceneContainer &sceneCont = contStorage.GetContainer<wb::ISceneContainer>();

        // Store the scene facade in the scene container
        sceneCont.Set(MockNextScneFacadeID(), std::move(sceneFacade));
    }
    
    // Load the next scene
    EXPECT_TRUE(sceneUpdator->NeedToLoad());
    sceneUpdator->SyncLoadNextScene
    (
        contStorage.GetContainer<wb::IAssetContainer>(), 
        contStorage.GetContainer<wb::ISceneContainer>()
    );

    std::cout << "Scene loaded successfully." << std::endl;

    // Update the current scene
    sceneUpdator->UpdateCurrentScene(contStorage);

    // Switch to the next scene
    EXPECT_TRUE(sceneUpdator->IsSwitching());
    sceneUpdator->AsyncLoadNextScene
    (
        contStorage.GetContainer<wb::IAssetContainer>(), 
        contStorage.GetContainer<wb::ISceneContainer>()
    );

    // Wait for the async load to finish
    while (!sceneUpdator->IsFinishedLoading())
    {
        // Update the current scene
        sceneUpdator->UpdateCurrentScene(contStorage);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Next scene loaded successfully." << std::endl;

    // Release the current scene
    sceneUpdator->AsyncReleaseCurrentScene
    (
        contStorage.GetContainer<wb::IAssetContainer>(), 
        contStorage.GetContainer<wb::ISceneContainer>()
    );

    // Move to the next scene
    sceneUpdator->MoveToNextScene();

    if (sceneUpdator->IsFinishedReleasing())
    {
        // Update the current scene
        sceneUpdator->UpdateCurrentScene(contStorage);
    }
    
    // Wait for the async release to finish
    while (!sceneUpdator->IsFinishedReleasing())
    {
        // Update the current scene
        sceneUpdator->UpdateCurrentScene(contStorage);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Before scene released successfully." << std::endl;

    EXPECT_TRUE(sceneUpdator->NeedToExit());

    // Sync release the current scene
    sceneUpdator->SyncReleaseCurrentScene
    (
        contStorage.GetContainer<wb::IAssetContainer>(), 
        contStorage.GetContainer<wb::ISceneContainer>()
    );

    std::cout << "Scene released successfully." << std::endl;
}