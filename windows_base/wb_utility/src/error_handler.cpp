#include "wb_utility/src/pch.h"

#include "wb_utility/include/error_handler.h"

WB_UTILITY_API void wb::ErrorNotify(const std::string &title, const std::string &message)
{
    MessageBoxA
    (
        nullptr,
        message.c_str(),
        title.c_str(),
        MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST
    );
}

WB_UTILITY_API void wb::QuitProgram(int exitCode)
{
    throw std::runtime_error("Quit Program with exit code: " + std::to_string(exitCode));
}