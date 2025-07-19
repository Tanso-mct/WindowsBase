#include "$project_name$/src/pch.h"
#include "$project_name$/include/scene_$scene_name$/scene.h"

#include "$project_name$/include/scene_$scene_name$/entities_factory.h"
#include "$project_name$/include/scene_$scene_name$/asset_group.h"
#include "$project_name$/include/scene_$scene_name$/system_scheduler.h"

const size_t &$project_name$::$SceneName$SceneFacadeID()
{
    static size_t id = wb::IDFactory::CreateSceneFacadeID();
    return id;
}

namespace $project_name$
{
    WB_REGISTER_SCENE_FACADE
    (
        $SceneName$SceneFacadeID,
        $SceneName$EntitiesFactory,
        $SceneName$AssetGroup,
        $SceneName$SystemScheduler
    );
}