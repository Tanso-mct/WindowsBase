#pragma once

#include "windows_base/include/id_factory.h"

namespace wb
{
    class SystemArgument
    {
    public:
        SystemArgument() = default;
        virtual ~SystemArgument() = default;

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