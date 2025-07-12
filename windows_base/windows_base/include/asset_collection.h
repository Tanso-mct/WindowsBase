#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/interfaces/collection.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"

namespace wb
{
    class WINDOWS_BASE_API AssetCollection : public IAssetCollection
    {
    private:
        std::unordered_map<size_t, std::unique_ptr<IAssetFactory>> assetFactories_;
        size_t maxId = 0;
        std::vector<size_t> keys_;

    public:
        AssetCollection() = default;
        virtual ~AssetCollection() = default;

        AssetCollection(const AssetCollection &) = delete;
        AssetCollection &operator=(const AssetCollection &) = delete;

        /***************************************************************************************************************
         * IAssetCollection implementation
        /**************************************************************************************************************/

        void AddFactory(size_t assetID, std::unique_ptr<IAssetFactory> assetFactory) override;
        IAssetFactory &GetFactory(size_t assetID) override;

        size_t GetMaxID() const override;
        const std::vector<size_t> &GetKeys() const override;
    };

    extern WINDOWS_BASE_API AssetCollection gAssetCollection;

    class WINDOWS_BASE_API AssetRegistrar
    {
    public:
        AssetRegistrar(size_t assetID, std::unique_ptr<IAssetFactory> assetFactory);
    };

    #define WB_REGISTER_ASSET(ASSET_FACTORY, ID) \
        static wb::AssetRegistrar assetRegistrar##T(ID, std::make_unique<ASSET_FACTORY>());


} // namespace wb