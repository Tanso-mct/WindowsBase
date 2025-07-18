#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    const size_t &DataAssetFactoryID();

    class DataAssetFactory : public wb::IAssetFactory
    {
    private:
        std::string data_;

    public:
        DataAssetFactory() = default;
        ~DataAssetFactory() override = default;

        std::unique_ptr<wb::IAsset> Create(wb::IFileData &fileData) const override;
    };
} // namespace example