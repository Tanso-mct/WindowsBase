#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/interfaces/container.h"
#include "windows_base/include/interfaces/entity.h"

namespace wb
{

    class WINDOWS_BASE_API SystemArgument
    {
    public:
        SystemArgument
        (
            IEntityContainer &entityContainer,
            IComponentContainer &componentContainer,
            IEntityIDView &entityIDView
        );
        ~SystemArgument() = default;

        IEntityContainer &entityContainer_;
        IComponentContainer &componentContainer_;
        IEntityIDView &entityIDView_;

        // TODO: Add any necessary data members or methods for system arguments
    };

} // namespace wb