#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/interfaces/asset.h"
#include "windows_base/include/asset_collection.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    const size_t &MockAssetID()
    {
        static size_t id = wb::IDFactory::CreateAssetID();
        return id;
    }

    const size_t &MockAssetFactoryID()
    {
        static size_t id = wb::IDFactory::CreateAssetFactoryID();
        return id;
    }

    const size_t &MockFileLoaderID()
    {
        static size_t id = wb::IDFactory::CreateFileLoaderID();
        return id;
    }

    constexpr const char* MOCK_FILE_PATH = "mock_file_path";
    WB_REGISTER_ASSET(MockAssetID(), MockAssetFactoryID(), MockFileLoaderID(), MOCK_FILE_PATH);
}

TEST(AssetCollection, GetAssetDetails)
{
    const size_t &assetFactoryID = wb::gAssetCollection.GetFactoryID(MockAssetID());
    EXPECT_EQ(assetFactoryID, MockAssetFactoryID());

    const size_t &fileLoaderID = wb::gAssetCollection.GetFileLoaderID(MockAssetID());
    EXPECT_EQ(fileLoaderID, MockFileLoaderID());

    std::string_view filePath = wb::gAssetCollection.GetFilePath(MockAssetID());
    EXPECT_EQ(std::string(filePath), std::string(MOCK_FILE_PATH));
}