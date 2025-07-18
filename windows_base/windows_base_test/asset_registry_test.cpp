#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/interfaces/asset.h"
#include "windows_base/include/asset_registry.h"
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
    WB_REGISTER_ASSET(MockAssetID, MockAssetFactoryID(), MockFileLoaderID(), MOCK_FILE_PATH);
}

TEST(AssetRegistry, GetAssetDetails)
{
    const size_t &assetFactoryID = wb::gAssetRegistry.GetFactoryID(MockAssetID());
    EXPECT_EQ(assetFactoryID, MockAssetFactoryID());

    const size_t &fileLoaderID = wb::gAssetRegistry.GetFileLoaderID(MockAssetID());
    EXPECT_EQ(fileLoaderID, MockFileLoaderID());

    std::string_view filePath = wb::gAssetRegistry.GetFilePath(MockAssetID());
    EXPECT_EQ(std::string(filePath), std::string(MOCK_FILE_PATH));
}