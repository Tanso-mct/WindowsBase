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
        MockAsset(const wb::IFileData &fileData) : fileData_(fileData) {}
        ~MockAsset() override = default;

        const size_t &GetID() const override
        {
            return MockAssetID();
        }

    private:
        const wb::IFileData &fileData_;
    };

    WB_REGISTER_ASSET(MockAsset, MockAssetID());
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