#pragma once

#include "windows_base/include/container_static.h"
#include "windows_base/include/container_dynamic.h"

#include "windows_base/include/interfaces/entity.h"
#include "windows_base/include/interfaces/component.h"
#include "windows_base/include/interfaces/system.h"
#include "windows_base/include/interfaces/asset.h"

namespace wb
{
    using EntityContainer = DynamicContainer<IEntity>;
    using ComponentContainer = DynamicContainer<IComponent>;
    using SystemContainer = StaticContainer<ISystem>;
    using AssetContainer = StaticContainer<IAsset>;

} // namespace wb