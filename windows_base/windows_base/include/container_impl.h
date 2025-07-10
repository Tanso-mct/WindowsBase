#pragma once

#include "windows_base/include/container_static.h"
#include "windows_base/include/container_dynamic.h"

#include "windows_base/include/entity.h"
#include "windows_base/include/system.h"
#include "windows_base/include/component.h"

namespace wb
{
    using EntityContainer = DynamicContainer<IEntity>;
    using ComponentContainer = DynamicContainer<IComponent>;
    using SystemContainer = StaticContainer<ISystem>;

} // namespace wb