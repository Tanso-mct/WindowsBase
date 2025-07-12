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

    class MockAsset : public wb::IAsset
    {
    public:
        MockAsset() = default;
        ~MockAsset() override = default;

        const size_t &GetID() const override
        {
            return MockAssetID();
        }
    };

    class MockAssetFactory : public wb::IAssetFactory
    {
    public:
        MockAssetFactory() = default;
        ~MockAssetFactory() override = default;

        std::unique_ptr<wb::IAsset> Create(const wb::IFileData &fileData) const override
        {
            // Mock implementation, returning a new MockAsset
            return std::make_unique<MockAsset>();
        }
    };

    WB_REGISTER_ASSET(MockAssetFactory, MockAssetID());
}

TEST(AssetCollection, GetFactory)
{
    wb::IAssetFactory &factory = wb::gAssetCollection.GetFactory(MockAssetID());
    EXPECT_NE(&factory, nullptr);

    std::unique_ptr<wb::IFileData> fileData = nullptr; // Mock file data
    std::unique_ptr<wb::IAsset> asset = factory.Create(*fileData);
    EXPECT_NE(asset, nullptr);
    EXPECT_EQ(asset->GetID(), MockAssetID());
}