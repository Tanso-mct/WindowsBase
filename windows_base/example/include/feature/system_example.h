#pragma once
#include "windows_base/windows_base.h"

namespace example
{
    class ExampleSystem : public wb::ISystem
    {
    public:
        ExampleSystem() = default;
        ~ExampleSystem() override = default;

        /***************************************************************************************************************
         * ISystem implementation
        /**************************************************************************************************************/

        const size_t &GetID() const override;
        void Initialize(wb::IAssetContainer &assetContainer) override;
        void Update(const wb::SystemArgument &args) override;
    };
} // namespace example