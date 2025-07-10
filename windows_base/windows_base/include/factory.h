#pragma once

#include "windows_base/include/entity.h"
#include "windows_base/include/component.h"
#include "windows_base/include/system.h"

namespace wb
{
    template <typename PRODUCT>
    class IFactory
    {
    public:
        virtual ~IFactory() = default;
        virtual PRODUCT Create() const = 0;
    };

    using IComponentFactory = IFactory<std::unique_ptr<IComponent>>;
    using ISystemFactory = IFactory<std::unique_ptr<ISystem>>;

}