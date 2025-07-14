#pragma once

#include "windows_base/include/interfaces/container.h"

#include <memory>

namespace wb
{
    template <typename PRODUCT, typename... ARGS>
    class IFactory
    {
    public:
        virtual ~IFactory() = default;
        virtual PRODUCT Create(ARGS...) const = 0;
    };

    class IComponent;
    using IComponentFactory = IFactory<std::unique_ptr<IComponent>>;

    using ISystemFactory = IFactory<std::unique_ptr<ISystem>, IAssetContainer&>;
    using ISystemsFactory = IFactory<std::unique_ptr<ISystemContainer>, IAssetContainer&>;

    class IEntityIDView;
    using IEntitiesFactory = IFactory<void, IAssetContainer&, IEntityContainer&, IComponentContainer&, IEntityIDView&>;

    class IEntityIDView;
    using IEntityIDViewFactory = IFactory<std::unique_ptr<IEntityIDView>>;

    class IFileData;
    using IAssetFactory = IFactory<std::unique_ptr<IAsset>, const IFileData &>;

    class ISceneFacade;
    class ISceneContext;
    class IAssetGroup;
    class ISystemScheduler;
    using ISceneFacadeFactory = IFactory
    <
        std::unique_ptr<ISceneFacade>,
        std::unique_ptr<ISceneContext>,
        std::unique_ptr<IEntitiesFactory>,
        std::unique_ptr<IEntityIDViewFactory>,
        std::unique_ptr<ISystemsFactory>,
        std::unique_ptr<IAssetGroup>,
        std::unique_ptr<ISystemScheduler>
    >;
}