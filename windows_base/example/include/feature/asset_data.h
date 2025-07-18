#pragma once
#include "windows_base/windows_base.h"

#include "example/include/feature/interfaces/asset_data.h"

namespace example
{
    class DataAsset : public example::IDataAsset
    {
    private:
        std::string data_;

    public:
        DataAsset() = default;
        ~DataAsset() override = default;

        std::string_view GetData() const override { return data_; }
        void SetData(const std::string &data) override { data_ = data; }
    };
} // namespace example