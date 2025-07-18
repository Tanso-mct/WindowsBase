#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    const size_t &TextFileLoaderID();

    class TextFileLoader : public wb::IFileLoader
    {
    public:
        TextFileLoader() = default;
        ~TextFileLoader() override = default;

        const size_t &GetID() const override;
        std::unique_ptr<wb::IFileData> Load(std::string_view path) override;
    };
} // namespace example