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

    using IEntitiesFactory = IFactory<void>;

    class IEntityIDView;
    using IEntityIDViewFactory = IFactory<std::unique_ptr<IEntityIDView>>;

    using ISystemsFactory = IFactory<std::unique_ptr<ISystemContainer>>;

}