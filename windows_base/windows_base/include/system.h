#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/interfaces/container.h"
#include "windows_base/include/interfaces/entity.h"
#include "windows_base/include/interfaces/scene.h"

namespace wb
{

    class WINDOWS_BASE_API SystemArgument
    {
    public:
        SystemArgument
        (
            IEntityContainer &entityContainer,
            IComponentContainer &componentContainer,
            IEntityIDView &entityIDView,
            ISystemContainer &systemContainer
        );
        ~SystemArgument() = default;

        IEntityContainer &entityContainer_;
        IComponentContainer &componentContainer_;
        IEntityIDView &entityIDView_;

        ISystemContainer &systemContainer_;

        SceneState state_ = SceneState::Size;

        // TODO: Add any necessary data members or methods for system arguments
    };

} // namespace wb