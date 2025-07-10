#pragma once

#include "windows_base/include/id_factory.h"

namespace wb
{
    class IComponent
    {
    public:
        virtual ~IComponent() = default;
        virtual size_t GetID() const = 0;
    };

} // namespace wb