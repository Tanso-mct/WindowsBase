#include "example/src/pch.h"
#include "example/include/feature/asset.h"

#include "example/include/feature/asset_factory_data.h"
#include "example/include/feature/file_loader_text.h"

const size_t &example::ExampleDataAssetID()
{
    static size_t id = wb::IDFactory::CreateAssetID();
    return id;
}

namespace example
{
    WB_REGISTER_ASSET
    (
        ExampleDataAssetID, DataAssetFactoryID(), TextFileLoaderID(),
        "../resources/example/data.txt"
    );
    
} // namespace example
