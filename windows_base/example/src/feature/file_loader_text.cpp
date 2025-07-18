#include "example/src/pch.h"
#include "example/include/feature/file_loader_text.h"

#include "example/include/feature/file_data_text.h"

const size_t &example::TextFileLoaderID()
{
    static size_t id = wb::IDFactory::CreateFileLoaderID();
    return id;
}

const size_t &example::TextFileLoader::GetID() const
{
    return TextFileLoaderID();
}

std::unique_ptr<wb::IFileData> example::TextFileLoader::Load(std::string_view path)
{
    // Create file data instance
    std::unique_ptr<example::ITextFileData> fileData = std::make_unique<example::TextFileData>();

    // Load text file
    fpos_t size = 0;
    std::unique_ptr<unsigned char[]> data = wb::LoadFileData(path, size);

    // Set content in file data
    std::string content(reinterpret_cast<const char*>(data.get()), size);
    fileData->SetContent(content);

    // Return as IFileData
    std::unique_ptr<wb::IFileData> rt = wb::UniqueAs<wb::IFileData>(fileData);
    return rt;
}

namespace example
{
    WB_REGISTER_FILE_LOADER(TextFileLoaderID(), TextFileLoader);
}

