#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class ITextFileData : public wb::IFileData
    {
    public:
        virtual ~ITextFileData() = default;

        virtual std::string_view GetContent() const = 0;
        virtual void SetContent(const std::string &content) = 0;
    };
}