#include "windows_base/src/pch.h"
#include "windows_base/include/system.h"

#include "windows_base/include/interfaces/system.h"
#include "windows_base/include/interfaces/container.h"
#include "windows_base/include/interfaces/entity.h"

wb::SystemArgument::SystemArgument
(
    IEntityContainer &entityContainer, IComponentContainer &componentContainer, 
    IEntityIDView &entityIDView, ISystemContainer &systemContainer
) :
    entityContainer_(entityContainer),
    componentContainer_(componentContainer),
    entityIDView_(entityIDView),
    systemContainer_(systemContainer)
{
}

