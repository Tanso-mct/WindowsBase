#pragma once

#include <initializer_list>
#include <string>
#include <iostream>
#include <Windows.h>

namespace wb
{
    constexpr const char* CONSOLE_TEXT_COLOR_NORMAL = "\033[0m";
    constexpr const char* CONSOLE_TEXT_COLOR_WARNING = "\033[33m";
    constexpr const char* CONSOLE_TEXT_COLOR_ERROR = "\033[31m";

    inline void ConsoleLog(const std::initializer_list<const char*> &messages)
    {
#ifndef NDEBUG
        std::cout << CONSOLE_TEXT_COLOR_NORMAL;
        for (const auto &message : messages)
        {
            // Windows Consoleにログを出力
            std::cout << message << std::endl;

            // Visual Studioのデバッグコンソールにログを出力
            OutputDebugStringA(message);
            OutputDebugStringA("\n");
        }
        std::cout << CONSOLE_TEXT_COLOR_NORMAL;
#endif
    }

    inline void ConsoleLogWrn(const std::initializer_list<const char*> &messages)
    {
#ifndef NDEBUG
        std::cout << CONSOLE_TEXT_COLOR_WARNING;
        for (const auto &message : messages)
        {
            // Windows Consoleにログを出力
            std::cout << message << std::endl;

            // Visual Studioのデバッグコンソールにログを出力
            OutputDebugStringA(message);
            OutputDebugStringA("\n");
        }
        std::cout << CONSOLE_TEXT_COLOR_NORMAL;
#endif
    }

    inline std::string CreateErrorMessage
    (
        const std::string& file, int line, const std::string& function,
        const std::initializer_list<const char*> &messages
    ){
        std::string errMsg = "";
        for (const auto &message : messages)
        {
            // メッセージを連結
            errMsg += message + std::string("\n");
        }

        errMsg += file + " : " + std::to_string(line) + "\n";
        errMsg += function + "\n";

        return errMsg;
    }

    inline std::string ConsoleLogErr
    (
        const std::string& file, int line, const std::string& function,
        const std::initializer_list<const char*> &messages
    ){
        
        std::string errMsg = CreateErrorMessage(file, line, function, messages);

#ifndef NDEBUG
        // Windows Consoleにエラーメッセージを出力
        std::cout << CONSOLE_TEXT_COLOR_ERROR;
        std::cout << errMsg;
        std::cout << CONSOLE_TEXT_COLOR_NORMAL;

        // Visual Studioのデバッグコンソールにエラーメッセージを出力
        OutputDebugStringA(errMsg.c_str());
#endif

        return errMsg;
    }
    
} // namespace wb