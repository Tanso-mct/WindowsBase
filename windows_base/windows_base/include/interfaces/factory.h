#pragma once

#include "windows_base/include/interfaces/container.h"

#include <memory>

namespace wb
{
    template <typename PRODUCT>
    class IFactory
    {
    public:
        virtual ~IFactory() = default;
        virtual PRODUCT Create() const = 0;
    };

    class IComponent;
    using IComponentFactory = IFactory<std::unique_ptr<IComponent>>;

    class ISystem;
    using ISystemFactory = IFactory<std::unique_ptr<ISystem>>;
    using ISystemsFactory = IFactory<std::unique_ptr<ISystemContainer>>;

    using IEntitiesFactory = IFactory<void>;

    class IEntityIDView;
    using IEntityIDViewFactory = IFactory<std::unique_ptr<IEntityIDView>>;

    class IFileData;
    class IAssetFactory
    {
    public:
        virtual ~IAssetFactory() = default;
        virtual std::unique_ptr<IAsset> Create(const IFileData &fileData) const = 0;
    };



}