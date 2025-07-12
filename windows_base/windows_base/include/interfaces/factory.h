#pragma once

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

}