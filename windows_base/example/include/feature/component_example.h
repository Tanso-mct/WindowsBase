#pragma once
#include "windows_base/windows_base.h"

#include "example/include/feature/interfaces/component_example.h"

namespace example
{
    const size_t &ExampleComponentID();

    const float DEFAULT_EXAMPLE_VALUE = 10.0f;

    class ExampleComponent : public IExampleComponent
    {
    private:
        float value_ = DEFAULT_EXAMPLE_VALUE;

        size_t dataAssetID_ = 0;
        mutable bool isDataAssetIDSet_ = false;

    public:
        ExampleComponent() = default;
        ~ExampleComponent() override = default;

        /***************************************************************************************************************
         * IComponent implementation
        /**************************************************************************************************************/

        const size_t &GetID() const override;

        /***************************************************************************************************************
         * IExampleComponent implementation
        /**************************************************************************************************************/

        const float &GetValue() const override { return value_; }
        void SetValue(float value) override { value_ = value; }

        const size_t &GetDataAssetID() const override;
        void SetDataAssetID(size_t dataAssetID) override;
    };
} // namespace example