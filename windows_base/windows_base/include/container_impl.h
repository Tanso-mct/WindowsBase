#pragma once

#include "windows_base/include/container_static.h"
#include "windows_base/include/container_dynamic.h"

namespace wb
{
    class IEntity;
    using EntityContainer = DynamicContainer<IEntity>;

    class IComponent;
    using ComponentContainer = DynamicContainer<IComponent>;

    class ISystem;
    using SystemContainer = StaticContainer<ISystem>;

} // namespace wb