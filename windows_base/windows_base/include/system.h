#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/container.h"
#include "windows_base/include/entity.h"

namespace wb
{

    class WINDOWS_BASE_API SystemArgument
    {
    public:
        SystemArgument
        (
            IEntityContainer &entityContainer,
            IComponentContainer &componentContainer,
            EntityIDView &entityIDView
        );
        ~SystemArgument() = default;

        IEntityContainer &entityContainer_;
        IComponentContainer &componentContainer_;
        EntityIDView &entityIDView_;

        // TODO: Add any necessary data members or methods for system arguments
    };

    class ISystem
    {
    public:
        virtual ~ISystem() = default;
        virtual size_t GetID() const = 0;
        virtual void Update(const SystemArgument &args) = 0;
    };

} // namespace wb