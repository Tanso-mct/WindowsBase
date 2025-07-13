#include "windows_base/src/pch.h"
#include "windows_base/include/system.h"

#include "windows_base/include/interfaces/system.h"
#include "windows_base/include/interfaces/container.h"
#include "windows_base/include/interfaces/entity.h"

wb::SystemArgument::SystemArgument
(
    IEntityContainer &entityContainer, IComponentContainer &componentContainer, 
    IEntityIDView &entityIDView, ISystemContainer &systemContainer,
    ContainerStorage &containerStorage,
    const double &deltaTime, const size_t &belongWindowID, size_t &nextSceneID
) :
    entityContainer_(entityContainer),
    componentContainer_(componentContainer),
    entityIDView_(entityIDView),
    systemContainer_(systemContainer),
    containerStorage_(containerStorage),
    deltaTime_(deltaTime),
    belongWindowID_(belongWindowID),
    nextSceneID_(nextSceneID)
{
}

