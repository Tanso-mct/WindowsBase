#pragma once

#include "wb_utility/include/dll_config.h"

#include <string>

namespace wb
{
    WB_UTILITY_API void ErrorNotify(const std::string& title, const std::string& message);
    WB_UTILITY_API void QuitProgram(int exitCode = 1);
}