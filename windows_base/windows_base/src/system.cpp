#include "windows_base/src/pch.h"
#include "windows_base/include/system.h"

wb::SystemArgument::SystemArgument
(
    IEntityContainer &entityContainer, IComponentContainer &componentContainer, 
    IEntityIDView &entityIDView
) :
    entityContainer_(entityContainer),
    componentContainer_(componentContainer),
    entityIDView_(entityIDView)
{
}