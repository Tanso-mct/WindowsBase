#include "example/src/pch.h"
#include "example/include/feature/asset_factory_data.h"

#include "example/include/feature/file_data_text.h"
#include "example/include/feature/asset_data.h"

const size_t &example::DataAssetFactoryID()
{
    static size_t id = wb::IDFactory::CreateAssetFactoryID();
    return id;
}

std::unique_ptr<wb::IAsset> example::DataAssetFactory::Create(wb::IFileData &fileData) const
{
    // Cast fileData to ITextFileData
    example::ITextFileData *textFileData = wb::As<example::ITextFileData>(&fileData);
    if (textFileData == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Invalid file data type for DataAsset creation."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("EXAMPLE", err);
        wb::ThrowRuntimeError(err);
    }

    // Create a new asset with the content from the text file data
    std::unique_ptr<example::IDataAsset> asset = std::make_unique<example::DataAsset>();
    asset->SetData(textFileData->GetContent().data());
    
    // Return as IAsset
    std::unique_ptr<wb::IAsset> rt = wb::UniqueAs<wb::IAsset>(asset);
    return rt;
}

namespace example
{
    WB_REGISTER_ASSET_FACTORY(DataAssetFactoryID(), DataAssetFactory);
}