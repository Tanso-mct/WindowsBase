#pragma once
#include "windows_base/include/dll_config.h"

namespace wb
{
    class WINDOWS_BASE_API IDFactory
    {
    public:
        IDFactory() = default;
        ~IDFactory() = default;

        static size_t CreateComponentID();

    };

} // namespace wb