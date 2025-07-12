#pragma once

#include <vector>

namespace wb
{
    class IAsset
    {
    public:
        virtual ~IAsset() = default;
        virtual const size_t &GetID() const = 0;
    };

    class IAssetGroup
    {
    public:
        virtual ~IAssetGroup() = default;
        virtual const std::vector<size_t> &GetAssetIDs() const = 0;

    protected:
        virtual void AddAssetID(size_t assetID) = 0;
    };

} // namespace wb