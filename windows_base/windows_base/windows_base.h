#pragma once
#include "windows_base/include/dll_config.h"

/***********************************************************************************************************************
 * Utility Headers
/**********************************************************************************************************************/

#include "windows_base/include/id_factory.h"
#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"
#include "windows_base/include/type_cast.h"
#include "windows_base/include/windows_helper.h"

/***********************************************************************************************************************
 * Container Headers
/**********************************************************************************************************************/

#include "windows_base/include/container_dynamic.h"
#include "windows_base/include/container_static.h"
#include "windows_base/include/container_storage.h"
#include "windows_base/include/container_impl.h"
#include "windows_base/include/optional_value.h"

/***********************************************************************************************************************
 * Event Headers
/**********************************************************************************************************************/

#include "windows_base/include/event.h"

/***********************************************************************************************************************
 * Window Headers
/**********************************************************************************************************************/

#include "windows_base/include/window.h"
#include "windows_base/include/window_collection.h"

/***********************************************************************************************************************
 * Monitor Headers
/**********************************************************************************************************************/

#include "windows_base/include/monitor_collection.h"
#include "windows_base/include/monitor_factory_collection.h"
#include "windows_base/include/monitor_keyboard.h"
#include "windows_base/include/monitor_mouse.h"

/***********************************************************************************************************************
 * Scene Headers
/**********************************************************************************************************************/

#include "windows_base/include/scene.h"
#include "windows_base/include/scene_facade_collection.h"

/***********************************************************************************************************************
 * Shared Headers
/**********************************************************************************************************************/

#include "windows_base/include/shared_facade_collection.h"

/***********************************************************************************************************************
 * Asset Headers
/**********************************************************************************************************************/

#include "windows_base/include/asset_collection.h"
#include "windows_base/include/asset_factory_collection.h"
#include "windows_base/include/asset_group.h"

/***********************************************************************************************************************
 * File Headers
/**********************************************************************************************************************/

#include "windows_base/include/file_loader_collection.h"

/***********************************************************************************************************************
 * ECS Headers
/**********************************************************************************************************************/

#include "windows_base/include/entity.h"
#include "windows_base/include/component_collection.h"
#include "windows_base/include/system.h"
#include "windows_base/include/system_collection.h"

/***********************************************************************************************************************
 * Windows Base Library
/**********************************************************************************************************************/

namespace wb
{
    struct LibraryConfig
    {
        WNDPROC windowProc_ = nullptr;

        std::vector<size_t> createWindowIDs_;
        std::vector<size_t> createMonitorIDs_;
        std::vector<size_t> createSceneIDs_;
        std::vector<size_t> createSharedIDs_;
    };

    class WINDOWS_BASE_API WindowsBaseLibrary
    {
    private:
        static bool isInitialized_;
        static EventInvoker<HWND, IWindowEvent, HWND, ContainerStorage&, UINT, WPARAM, LPARAM> windowEventInvoker_;
        static ContainerStorage containerStorage_;

        /***************************************************************************************************************
         * WindowsBaseLibrary is a singleton class so constructor and destructor are private.
        /**************************************************************************************************************/

        WindowsBaseLibrary() = default;
        ~WindowsBaseLibrary() = default;

        WindowsBaseLibrary(const WindowsBaseLibrary&) = delete;
        WindowsBaseLibrary& operator=(const WindowsBaseLibrary&) = delete;

    public:
        static void Initialize(LibraryConfig &config);
        static void Run();
        static void Shutdown();

        static void HandleWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };

} // namespace wb