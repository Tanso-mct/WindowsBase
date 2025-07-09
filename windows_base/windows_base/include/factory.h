#pragma once

#include "windows_base/include/component.h"

namespace wb
{
    template <typename PRODUCT>
    class IFactory
    {
    public:
        virtual ~IFactory() = default;
        virtual std::unique_ptr<PRODUCT> Create() const = 0;
    };

    using IComponentFactory = IFactory<IComponent>;

}