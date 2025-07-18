#pragma once
#include "windows_base/windows_base.h"

#include "example/include/feature/interfaces/file_data_text.h"

namespace example
{
    class TextFileData : public example::ITextFileData
    {
    private:
        std::string content_;

    public:
        TextFileData() = default;
        ~TextFileData() override = default;

        std::string_view GetContent() const override { return content_; }
        void SetContent(const std::string &content) override { content_ = content; }
    };
} // namespace example