#include "example/src/pch.h"
#include "example/include/scene_example/system_scheduler.h"

#include "example/include/feature/system_example.h"

void example::ExampleSystemScheduler::Execute(wb::ISystemContainer &systemCont, wb::SystemArgument &args)
{
    systemCont.Get(example::ExampleSystemID()).Update(args);
}