#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/asset_factory_collection.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    const size_t &MockAssetFactoryID()
    {
        static size_t id = wb::IDFactory::CreateAssetFactoryID();
        return id;
    }

    class MockAssetFactory : public wb::IAssetFactory
    {
    public:
        MockAssetFactory() = default;
        virtual ~MockAssetFactory() override = default;

        std::unique_ptr<wb::IAsset> Create(const wb::IFileData &fileData) const override
        {
            // Mock implementation for testing purposes
            return nullptr;
        }
    };

    WB_REGISTER_ASSET_FACTORY(MockAssetFactoryID(), MockAssetFactory);
}

TEST(AssetFactoryCollection, GetFactory)
{
    wb::IAssetFactory &factory = wb::gAssetFactoryCollection.GetFactory(MockAssetFactoryID());
    EXPECT_NE(&factory, nullptr);
}

