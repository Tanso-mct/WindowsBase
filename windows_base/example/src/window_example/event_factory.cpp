#include "example/src/pch.h"
#include "example/include/window_example/event_factory.h"

#include "example/include/window_example/window.h"
#include "example/include/window_example/monitor_keyboard.h"
#include "example/include/window_example/monitor_mouse.h"

#include "example/include/scene_example/scene.h"

std::unique_ptr<wb::IWindowEvent> example::ExampleWindowEventFactory::Create() const
{
    std::unique_ptr<wb::IWindowEvent> event = std::make_unique<wb::DefaultWindowEvent>();

    // Set the window ID and other necessary properties
    event->SetWindowID(example::ExampleWindowID());

    {
        std::unique_ptr<wb::ISceneUpdator> sceneUpdator = std::make_unique<wb::SceneUpdator>();
        sceneUpdator->SetBelongWindowID(example::ExampleWindowID());
        sceneUpdator->SetInitialSceneID(example::ExampleSceneFacadeID());

        event->SetSceneUpdator(std::move(sceneUpdator));
    }

    event->SetKeyboardMonitorID(example::ExampleKeyboardMonitorID());
    event->SetMouseMonitorID(example::ExampleMouseMonitorID());

    return event;
}