#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/asset_factory_registry.h"
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

        std::unique_ptr<wb::IAsset> Create(wb::IFileData &fileData) const override
        {
            // Mock implementation for testing purposes
            return nullptr;
        }
    };

    WB_REGISTER_ASSET_FACTORY(MockAssetFactoryID(), MockAssetFactory);
}

TEST(AssetFactoryRegistry, GetFactory)
{
    wb::IAssetFactory &factory = wb::gAssetFactoryRegistry.GetFactory(MockAssetFactoryID());
    EXPECT_NE(&factory, nullptr);
}

