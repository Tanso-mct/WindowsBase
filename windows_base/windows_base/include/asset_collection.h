#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/interfaces/collection.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"

namespace wb
{
    // class WINDOWS_BASE_API AssetCollection : public IAssetCollection
    // {
    // private:
    //     std::unordered_map<size_t, std::unique_ptr<IAssetFactory>> assetFactories_;
    //     size_t maxId = 0;
    //     std::vector<size_t> keys_;

    // public:
    //     AssetCollection() = default;
    //     virtual ~AssetCollection() = default;

    //     AssetCollection(const AssetCollection &) = delete;
    //     AssetCollection &operator=(const AssetCollection &) = delete;

    //     /***************************************************************************************************************
    //      * IAssetCollection implementation
    //     /**************************************************************************************************************/

    //     void AddFactory(size_t assetID, std::unique_ptr<IAssetFactory> assetFactory) override;
    //     IAssetFactory &GetFactory(size_t assetID) override;

    //     size_t GetMaxID() const override;
    //     const std::vector<size_t> &GetKeys() const override;
    // };

    // WINDOWS_BASE_API AssetCollection& GetAssetCollectionInstance();

    // template <typename ASSET>
    // class AssetFactory : public IAssetFactory
    // {
    // public:
    //     AssetFactory() = default;
    //     virtual ~AssetFactory() override = default;

    //     std::unique_ptr<IAsset> Create(const IFileData &fileData) const override
    //     {
    //         return std::make_unique<ASSET>(fileData);
    //     }
    // };

    // template <typename ASSET>
    // class AssetRegistrar
    // {
    // public:
    //     AssetRegistrar(size_t assetID)
    //     {
    //         static bool registered = false;
    //         if (registered)
    //         {
    //             std::string err = CreateErrorMessage
    //             (
    //                 __FILE__, __LINE__, __FUNCTION__,
    //                 {"Asset with ID ", std::to_string(assetID), " is already registered."}
    //             );

    //             wb::ConsoleLogErr(err);
    //             wb::ErrorNotify("WINDOWS_BASE", err);
    //             wb::ThrowRuntimeError(err);
    //         }

    //         registered = true;

    //         AssetCollection &collection = GetAssetCollectionInstance();
    //         collection.AddFactory(assetID, std::make_unique<AssetFactory<ASSET>>());
    //     }
    // };

    // #define WB_REGISTER_ASSET(ASSET, ID) static wb::AssetRegistrar<ASSET> assetRegistrar##T(ID)


} // namespace wb