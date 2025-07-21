#include "example/src/pch.h"
#include "example/include/feature/component_example.h"

const size_t &example::ExampleComponentID()
{
    static size_t id = wb::IDFactory::CreateComponentID();
    return id;
}

const size_t &example::ExampleComponent::GetID() const
{
    return ExampleComponentID();
}

const size_t &example::ExampleComponent::GetDataAssetID() const
{
    if (!isDataAssetIDSet_)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Data asset ID is not set for component with ID ", std::to_string(GetID())}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("EXAMPLE", err);
        wb::ThrowRuntimeError(err);
    }

    return dataAssetID_;
}

void example::ExampleComponent::SetDataAssetID(size_t dataAssetID)
{
    dataAssetID_ = dataAssetID;
    isDataAssetIDSet_ = true;
}

namespace example
{
    WB_REGISTER_COMPONENT(ExampleComponentID(), ExampleComponent);
}