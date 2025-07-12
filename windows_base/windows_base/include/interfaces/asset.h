#pragma once

#include <vector>

namespace wb
{
    class IAssetGroup
    {
    public:
        virtual ~IAssetGroup() = default;
        virtual const std::vector<size_t> &GetAssetIDs() const = 0;
    };

} // namespace wb