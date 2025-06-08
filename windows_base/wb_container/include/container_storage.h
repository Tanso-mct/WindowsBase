#pragma once

#include "wb_interface/container.h"
#include "wb_interface/data_config.h"
#include "wb_interface/window.h"
#include "wb_interface/scene.h"
#include "wb_interface/common.h"
#include "wb_interface/monitor.h"
#include "wb_interface/asset.h"

#include "wb_container/include/static_container.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

#include <vector>
#include <memory>

namespace wb
{

    using IWindowContainer = IStaticContainer<IWindowFacade, WindowID>;
    using WindowContainer = StaticContainer<IWindowFacade, WindowID>;

    using ISceneContainer = IStaticContainer<ISceneFacade, SceneID>;
    using SceneContainer = StaticContainer<ISceneFacade, SceneID>;

    using ICommonContainer = IStaticContainer<ICommonFacade, CommonID>;
    using CommonContainer = StaticContainer<ICommonFacade, CommonID>;

    using IMonitorContainer = IStaticContainer<IMonitor, MonitorID>;
    using MonitorContainer = StaticContainer<IMonitor, MonitorID>;

    using IAssetContainer = IStaticContainer<IStaticContainer<IAsset, size_t>, AssetType>;
    using AssetContainer = StaticContainer<IStaticContainer<IAsset, size_t>, AssetType>;

    class ContainerStorage
    {
    private:
        std::unique_ptr<IWindowContainer> windowCont_ = nullptr;
        std::unique_ptr<ISceneContainer> sceneCont_ = nullptr;
        std::unique_ptr<ICommonContainer> commonCont_ = nullptr;
        std::unique_ptr<IMonitorContainer> monitorCont_ = nullptr;
        std::unique_ptr<IAssetContainer> assetCont_ = nullptr;

    public:
        ContainerStorage() = default;
        ~ContainerStorage() = default;

        template <typename T>
        std::unique_ptr<T>& Get()
        {
            std::string err = wb::ConsoleLogErr
            (
                __FILE__, __LINE__, __FUNCTION__,
                {"ContainerStorage Get : 無効なコンテナ型です" }
            );
            wb::ErrorNotify("WB_CONTAINER", err);
            wb::QuitProgram();
        }

        template <>
        std::unique_ptr<IWindowContainer>& Get<IWindowContainer>()
        {
            return windowCont_;
        }

        template <>
        std::unique_ptr<ISceneContainer>& Get<ISceneContainer>()
        {
            return sceneCont_;
        }

        template <>
        std::unique_ptr<ICommonContainer>& Get<ICommonContainer>()
        {
            return commonCont_;
        }

        template <>
        std::unique_ptr<IMonitorContainer>& Get<IMonitorContainer>()
        {
            return monitorCont_;
        }

        template <>
        std::unique_ptr<IAssetContainer>& Get<IAssetContainer>()
        {
            return assetCont_;
        }
    };

} // namespace wb