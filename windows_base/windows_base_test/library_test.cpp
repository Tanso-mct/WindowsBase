#include "pch.h"

#include "windows_base\windows_base.h"
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
            args.state_ = wb::SceneState::Updating;
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

    /*******************************************************************************************************************
     * Mock Mouse Monitor ID
    /******************************************************************************************************************/

    const size_t &MockMouseMonitorID()
    {
        static size_t id = wb::IDFactory::CreateMonitorID();
        return id;
    }

    /*******************************************************************************************************************
     * Register Mock Mouse Monitor
    /******************************************************************************************************************/

    WB_REGISTER_MONITOR(MockMouseMonitorID, wb::DefaultMouseMonitorFactoryID());

    /*******************************************************************************************************************
     * Mock Keyboard Monitor ID
    /******************************************************************************************************************/

    const size_t &MockKeyboardMonitorID()
    {
        static size_t id = wb::IDFactory::CreateMonitorID();
        return id;
    }

    /*******************************************************************************************************************
     * Register Mock Keyboard Monitor
    /******************************************************************************************************************/

    WB_REGISTER_MONITOR(MockKeyboardMonitorID, wb::DefaultKeyboardMonitorFactoryID());

    /*******************************************************************************************************************
     * Mock Window ID
    /******************************************************************************************************************/

    const size_t &MockWindowID()
    {
        static size_t id = wb::IDFactory::CreateWindowID();
        return id;
    }

    /*******************************************************************************************************************
     * Mock Window Facade Factory
    /******************************************************************************************************************/

    class MockWindowFacadeFactory : public wb::IWindowFacadeFactory
    {
    public:
        MockWindowFacadeFactory() = default;
        ~MockWindowFacadeFactory() override = default;

        std::unique_ptr<wb::IWindowFacade> Create() const override
        {
            std::unique_ptr<wb::IWindowFacade> facade = std::make_unique<wb::DefaultWindowFacade>();

            {
                std::unique_ptr<wb::IWindowContext> context = std::make_unique<wb::WindowContext>();
                context->Name() = L"Mock Window";

                facade->SetContext(std::move(context));
            }

            facade->AddMonitorID(MockKeyboardMonitorID());
            facade->AddMonitorID(MockMouseMonitorID());

            return facade;
        }
    };

    /*******************************************************************************************************************
     * Mock Window Event Factory
    /******************************************************************************************************************/

    class MockWindowEventFactory : public wb::IWindowEventFactory
    {
    public:
        MockWindowEventFactory() = default;
        ~MockWindowEventFactory() override = default;

        std::unique_ptr<wb::IWindowEvent> Create() const override
        {
            std::unique_ptr<wb::IWindowEvent> event = std::make_unique<wb::DefaultWindowEvent>();

            // Set the window ID and other necessary properties
            event->SetWindowID(MockWindowID());

            {
                std::unique_ptr<wb::ISceneUpdator> sceneUpdator = std::make_unique<wb::SceneUpdator>();
                sceneUpdator->SetBelongWindowID(MockWindowID());
                sceneUpdator->SetInitialSceneID(MockSceneFacadeID());

                event->SetSceneUpdator(std::move(sceneUpdator));
            }
            
            event->SetKeyboardMonitorID(MockKeyboardMonitorID());
            event->SetMouseMonitorID(MockMouseMonitorID());

            return event;
        }
    };

    /*******************************************************************************************************************
     * Register Mock Window
    /******************************************************************************************************************/

    WB_REGISTER_WINDOW(MockWindowID, MockWindowFacadeFactory, MockWindowEventFactory)    
}

static LRESULT CALLBACK WindowProcWithEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    wb::WindowsBaseLibrary::HandleWindowProc(hWnd, msg, wParam, lParam);

    switch (msg)
    {
    case WM_PAINT:
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

TEST(WindowsBaseLibrary, Use)
{
    // Initialize the WindowsBaseLibrary
    wb::LibraryConfig config;
    config.windowProc_ = WindowProcWithEvent;

    // Add window, monitor, scene, and shared IDs to the config
    config.createWindowIDs_.push_back(MockWindowID());
    config.createMonitorIDs_.push_back(MockMouseMonitorID());
    config.createMonitorIDs_.push_back(MockKeyboardMonitorID());
    config.createSceneIDs_.push_back(MockSceneFacadeID());

    wb::WindowsBaseLibrary::Initialize(config);

    // Run the WindowsBaseLibrary
    wb::WindowsBaseLibrary::Run();

    // Shutdown the WindowsBaseLibrary
    wb::WindowsBaseLibrary::Shutdown();

    return;
}