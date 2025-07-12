#include "windows_base/src/pch.h"
#include "windows_base/include/asset_collection.h"

void wb::AssetCollection::AddFactory(size_t assetID, std::unique_ptr<IAssetFactory> assetFactory)
{
    if (assetFactories_.find(assetID) != assetFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Asset factory with ID ", std::to_string(assetID), " already exists."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // Add the factory to the collection
    assetFactories_[assetID] = std::move(assetFactory);

    // Save the key
    keys_.push_back(assetID);

    // Update the maximum ID if necessary
    if (assetID > maxId)
    {
        maxId = assetID;
    }
}

wb::IAssetFactory &wb::AssetCollection::GetFactory(size_t assetID)
{
    if (assetFactories_.find(assetID) == assetFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Asset factory with ID ", std::to_string(assetID), " does not exist."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *assetFactories_[assetID];
}

size_t wb::AssetCollection::GetMaxID() const
{
    return maxId;
}

const std::vector<size_t> &wb::AssetCollection::GetKeys() const
{
    return keys_;
}

WINDOWS_BASE_API wb::AssetCollection wb::gAssetCollection;

wb::AssetRegistrar::AssetRegistrar(size_t assetID, std::unique_ptr<IAssetFactory> assetFactory)
{
    wb::gAssetCollection.AddFactory(assetID, std::move(assetFactory));
}