#include "example/src/pch.h"
#include "example/include/scene_example/scene.h"

#include "example/include/scene_example/entities_factory.h"
#include "example/include/scene_example/asset_group.h"
#include "example/include/scene_example/system_scheduler.h"

const size_t &example::ExampleSceneFacadeID()
{
    static size_t id = wb::IDFactory::CreateSceneFacadeID();
    return id;
}

namespace example
{
    WB_REGISTER_SCENE_FACADE
    (
        ExampleSceneFacadeID,
        ExampleEntitiesFactory,
        ExampleAssetGroup,
        ExampleSystemScheduler
    );
}