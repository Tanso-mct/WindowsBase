#include "pch.h"

#include "windows_base/include/id_factory.h"

#include "windows_base/include/monitor_mouse.h"
#include "windows_base/include/monitor_keyboard.h"
#include "windows_base/include/monitor_factory_collection.h"
#include "windows_base/include/monitor_collection.h"

#include "windows_base/include/scene.h"
#include "windows_base/include/entity.h"
#include "windows_base/include/container_impl.h"
#include "windows_base/include/asset_group.h"
#include "windows_base/include/file_loader_collection.h"
#include "windows_base/include/asset_factory_collection.h"
#include "windows_base/include/asset_collection.h"
#include "windows_base/include/container_storage.h"
#include "windows_base/include/system.h"
#include "windows_base/include/scene_facade_collection.h"

#include "windows_base/include/window_collection.h"
#include "windows_base/include/window.h"
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

            facade->AddMonitorID(wb::DefaultKeyboardMonitorFactoryID());
            facade->AddMonitorID(wb::DefaultMouseMonitorFactoryID());

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
            
            event->SetKeyboardMonitorID(wb::DefaultKeyboardMonitorFactoryID());
            event->SetMouseMonitorID(wb::DefaultMouseMonitorFactoryID());

            return event;
        }
    };

    /*******************************************************************************************************************
     * Register Mock Window
    /******************************************************************************************************************/

    WB_REGISTER_WINDOW(MockWindowID, MockWindowFacadeFactory, MockWindowEventFactory)    
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
        // WM_PAINTでDefWindowProcを返すと更新が止まるので、WM_PAINTは0を返す。
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

TEST(Window, Create)
{
    // Get the facade factory
    wb::IWindowFacadeFactory &facadeFactory = wb::gWindowCollection.GetFacadeFactory(MockWindowID());
    EXPECT_NE(&facadeFactory, nullptr);

    // Create a window facade using the factory
    std::unique_ptr<wb::IWindowFacade> windowFacade = facadeFactory.Create();
    EXPECT_NE(windowFacade, nullptr);

    // Check if the window is ready
    EXPECT_TRUE(windowFacade->CheckIsReady());

    // Create Window
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = windowFacade->GetName().data();
    windowFacade->Create(wc);

    // Check if the window is created
    EXPECT_TRUE(windowFacade->IsCreated());

    // Show the window
    windowFacade->Show();

    // Destroy the window
    windowFacade->Destroy();

    // Invoke the Destroyed method
    windowFacade->Destroyed();
}

TEST(Window, Hide)
{
    // Get the facade factory
    wb::IWindowFacadeFactory &facadeFactory = wb::gWindowCollection.GetFacadeFactory(MockWindowID());
    EXPECT_NE(&facadeFactory, nullptr);

    // Create a window facade using the factory
    std::unique_ptr<wb::IWindowFacade> windowFacade = facadeFactory.Create();
    EXPECT_NE(windowFacade, nullptr);

    // Check if the window is ready
    EXPECT_TRUE(windowFacade->CheckIsReady());

    // Create Window
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = windowFacade->GetName().data();
    windowFacade->Create(wc);

    // Show the window
    windowFacade->Show();

    // Hide the window
    windowFacade->Hide();

    // Destroy the window
    windowFacade->Destroy();

    // Invoke the Destroyed method
    windowFacade->Destroyed();
}

TEST(Window, Resize)
{
    // Get the facade factory
    wb::IWindowFacadeFactory &facadeFactory = wb::gWindowCollection.GetFacadeFactory(MockWindowID());
    EXPECT_NE(&facadeFactory, nullptr);

    // Create a window facade using the factory
    std::unique_ptr<wb::IWindowFacade> windowFacade = facadeFactory.Create();
    EXPECT_NE(windowFacade, nullptr);

    // Check if the window is ready
    EXPECT_TRUE(windowFacade->CheckIsReady());

    // Create Window
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = windowFacade->GetName().data();
    windowFacade->Create(wc);

    // Show the window
    windowFacade->Show();

    // Resize the window
    UINT newWidth = 250;
    UINT newHeight = 250;
    windowFacade->Resize(newWidth, newHeight);

    // Invoke the Resized method
    windowFacade->Resized();

    // Destroy the window
    windowFacade->Destroy();

    // Invoke the Destroyed method
    windowFacade->Destroyed();
}

TEST(Window, Focus)
{
    // Get the facade factory
    wb::IWindowFacadeFactory &facadeFactory = wb::gWindowCollection.GetFacadeFactory(MockWindowID());
    EXPECT_NE(&facadeFactory, nullptr);

    // Create a window facade using the factory
    std::unique_ptr<wb::IWindowFacade> windowFacade = facadeFactory.Create();
    EXPECT_NE(windowFacade, nullptr);

    // Check if the window is ready
    EXPECT_TRUE(windowFacade->CheckIsReady());

    // Create Window
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = windowFacade->GetName().data();
    windowFacade->Create(wc);

    // Show the window
    windowFacade->Show();

    // Focus the window
    windowFacade->Focus();

    // Invoke the Focused method
    windowFacade->Focused();

    // Destroy the window
    windowFacade->Destroy();

    // Invoke the Destroyed method
    windowFacade->Destroyed();
}

TEST(Window, Maximize)
{
    // Get the facade factory
    wb::IWindowFacadeFactory &facadeFactory = wb::gWindowCollection.GetFacadeFactory(MockWindowID());
    EXPECT_NE(&facadeFactory, nullptr);

    // Create a window facade using the factory
    std::unique_ptr<wb::IWindowFacade> windowFacade = facadeFactory.Create();
    EXPECT_NE(windowFacade, nullptr);

    // Check if the window is ready
    EXPECT_TRUE(windowFacade->CheckIsReady());

    // Create Window
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = windowFacade->GetName().data();
    windowFacade->Create(wc);

    // Show the window
    windowFacade->Show();

    // Maximize the window
    windowFacade->Maximize();

    // Invoke the Maximized method
    windowFacade->Maximized();

    // Destroy the window
    windowFacade->Destroy();

    // Invoke the Destroyed method
    windowFacade->Destroyed();
}

TEST(Window, Minimize)
{
    // Get the facade factory
    wb::IWindowFacadeFactory &facadeFactory = wb::gWindowCollection.GetFacadeFactory(MockWindowID());
    EXPECT_NE(&facadeFactory, nullptr);

    // Create a window facade using the factory
    std::unique_ptr<wb::IWindowFacade> windowFacade = facadeFactory.Create();
    EXPECT_NE(windowFacade, nullptr);

    // Check if the window is ready
    EXPECT_TRUE(windowFacade->CheckIsReady());

    // Create Window
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = windowFacade->GetName().data();
    windowFacade->Create(wc);

    // Show the window
    windowFacade->Show();

    // Minimize the window
    windowFacade->Minimize();

    // Invoke the Minimized method
    windowFacade->Minimized();

    // Destroy the window
    windowFacade->Destroy();

    // Invoke the Destroyed method
    windowFacade->Destroyed();
}

TEST(Window, FullScreen)
{
    // Get the facade factory
    wb::IWindowFacadeFactory &facadeFactory = wb::gWindowCollection.GetFacadeFactory(MockWindowID());
    EXPECT_NE(&facadeFactory, nullptr);

    // Create a window facade using the factory
    std::unique_ptr<wb::IWindowFacade> windowFacade = facadeFactory.Create();
    EXPECT_NE(windowFacade, nullptr);

    // Check if the window is ready
    EXPECT_TRUE(windowFacade->CheckIsReady());

    // Create Window
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = windowFacade->GetName().data();
    windowFacade->Create(wc);

    // Show the window
    windowFacade->Show();

    // Set full screen mode
    windowFacade->FullScreen();

    // Invoke the FullScreened method
    windowFacade->FullScreened();

    // Destroy the window
    windowFacade->Destroy();

    // Invoke the Destroyed method
    windowFacade->Destroyed();
}

TEST(Window, Restore)
{
    // Get the facade factory
    wb::IWindowFacadeFactory &facadeFactory = wb::gWindowCollection.GetFacadeFactory(MockWindowID());
    EXPECT_NE(&facadeFactory, nullptr);

    // Create a window facade using the factory
    std::unique_ptr<wb::IWindowFacade> windowFacade = facadeFactory.Create();
    EXPECT_NE(windowFacade, nullptr);

    // Check if the window is ready
    EXPECT_TRUE(windowFacade->CheckIsReady());

    // Create Window
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = windowFacade->GetName().data();
    windowFacade->Create(wc);

    // Show the window
    windowFacade->Show();

    // Restore the window
    windowFacade->Restore();

    // Invoke the Restored method
    windowFacade->Restored();

    // Destroy the window
    windowFacade->Destroy();

    // Invoke the Destroyed method
    windowFacade->Destroyed();
}

TEST(Window, Move)
{
    // Get the facade factory
    wb::IWindowFacadeFactory &facadeFactory = wb::gWindowCollection.GetFacadeFactory(MockWindowID());
    EXPECT_NE(&facadeFactory, nullptr);

    // Create a window facade using the factory
    std::unique_ptr<wb::IWindowFacade> windowFacade = facadeFactory.Create();
    EXPECT_NE(windowFacade, nullptr);

    // Check if the window is ready
    EXPECT_TRUE(windowFacade->CheckIsReady());

    // Create Window
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = windowFacade->GetName().data();
    windowFacade->Create(wc);

    // Show the window
    windowFacade->Show();

    // Move the window to a new position
    UINT newPosX = 100;
    UINT newPosY = 100;
    windowFacade->Move(newPosX, newPosY);

    // Invoke the Moved method
    windowFacade->Moved();

    // Destroy the window
    windowFacade->Destroy();

    // Invoke the Destroyed method
    windowFacade->Destroyed();
}