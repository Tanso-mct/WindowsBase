#include "windows_base/src/pch.h"
#include "windows_base/include/asset_group.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"

const std::vector<size_t> &wb::AssetGroup::GetAssetIDs() const
{
    return assetIDs_;
}

void wb::AssetGroup::AddAssetID(size_t assetID)
{
    // Check if the assetID already exists
    if (std::find(assetIDs_.begin(), assetIDs_.end(), assetID) != assetIDs_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Asset ID ", std::to_string(assetID), " already exists in the group."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // Add the new assetID to the group
    assetIDs_.emplace_back(assetID);
}