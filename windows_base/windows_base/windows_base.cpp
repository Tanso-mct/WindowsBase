#include "windows_base/src/pch.h"
#include "windows_base/windows_base.h"

bool isInitialized_ = false;
wb::EventInvoker<HWND, wb::IWindowEvent, HWND, wb::ContainerStorage&, UINT, WPARAM, LPARAM> windowEventInvoker_;
wb::ContainerStorage containerStorage_;

inline void wb::WindowsBaseLibrary::Initialize(LibraryConfig &config)
{
    if (isInitialized_)
    {
        std::string err = CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"WindowsBaseLibrary is already initialized."}
        );

        ConsoleLogErr(err);
        ErrorNotify("WINDOWS_BASE", err);
        ThrowRuntimeError(err);
    }

    /*******************************************************************************************************************
     * Create standard console when not in debug mode
    /******************************************************************************************************************/

#ifndef NDEBUG
    wb::CreateStandardConsole();
#endif

    /*******************************************************************************************************************
     * Create containers
    /******************************************************************************************************************/

    {
        // Create window container
        std::unique_ptr<wb::IWindowContainer> windowCont = std::make_unique<wb::WindowContainer>();
        windowCont->Create(wb::gWindowCollection.GetMaxID() + 1);
        containerStorage_.SetContainer<wb::IWindowContainer>(std::move(windowCont));

        // Create monitor container
        std::unique_ptr<wb::IMonitorContainer> monitorCont = std::make_unique<wb::MonitorContainer>();
        monitorCont->Create(wb::gMonitorCollection.GetMaxID() + 1);
        containerStorage_.SetContainer<wb::IMonitorContainer>(std::move(monitorCont));

        // Create scene container
        std::unique_ptr<wb::ISceneContainer> sceneCont = std::make_unique<wb::SceneContainer>();
        sceneCont->Create(wb::gSceneFacadeCollection.GetMaxID() + 1);
        containerStorage_.SetContainer<wb::ISceneContainer>(std::move(sceneCont));

        // Create shared container
        std::unique_ptr<wb::ISharedContainer> sharedCont = std::make_unique<wb::SharedContainer>();
        sharedCont->Create(wb::gSharedFacadeCollection.GetMaxID() + 1);
        containerStorage_.SetContainer<wb::ISharedContainer>(std::move(sharedCont));

        // Create asset container
        std::unique_ptr<wb::IAssetContainer> assetCont = std::make_unique<wb::AssetContainer>();
        assetCont->Create(wb::gAssetCollection.GetMaxID() + 1);
        containerStorage_.SetContainer<wb::IAssetContainer>(std::move(assetCont));
    }

}