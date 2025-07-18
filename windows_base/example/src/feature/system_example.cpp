#include "example/src/pch.h"
#include "example/include/feature/system_example.h"

#include "example/include/feature/component_example.h"
#include "example/include/feature/interfaces/asset_data.h"

const size_t &example::ExampleSystemID()
{
    static size_t id = wb::IDFactory::CreateSystemID();
    return id;
}

const size_t &example::ExampleSystem::GetID() const
{
    return ExampleSystemID();
}

void example::ExampleSystem::Initialize(wb::IAssetContainer &assetContainer)
{
    std::string message = wb::CreateMessage
    ({
        "ExampleSystem initialized",
        "Push 'Space' to log component values and data asset content.",
    });
    wb::ConsoleLog(message);
}

void example::ExampleSystem::Update(const wb::SystemArgument &args)
{
    // Get containers to use
    wb::IWindowContainer &windowContainer = args.containerStorage_.GetContainer<wb::IWindowContainer>();
    wb::IMonitorContainer &monitorContainer = args.containerStorage_.GetContainer<wb::IMonitorContainer>();
    wb::IAssetContainer &assetContainer = args.containerStorage_.GetContainer<wb::IAssetContainer>();

    // Get the window facade for the current window
    wb::IWindowFacade &window = windowContainer.Get(args.belongWindowID_);

    // Get the keyboard and mouse monitors
    wb::IKeyboardMonitor *keyboardMonitor = nullptr;
    wb::IMouseMonitor *mouseMonitor = nullptr;
    for (const size_t &monitorID : window.GetMonitorIDs())
    {
        wb::IMonitor &monitor = monitorContainer.Get(monitorID);
        if (keyboardMonitor == nullptr) keyboardMonitor = wb::As<wb::IKeyboardMonitor>(&monitor);
        if (mouseMonitor == nullptr) mouseMonitor = wb::As<wb::IMouseMonitor>(&monitor);
    }

    for (const std::unique_ptr<wb::IOptionalValue> &id : (args.entityIDView_)(ExampleComponentID()))
    {
        wb::IEntity *entity = args.entityContainer_.PtrGet(*id);
        if (entity == nullptr) continue; // Skip if entity is null

        wb::IComponent *component = entity->GetComponent(ExampleComponentID(), args.componentContainer_);
        example::IExampleComponent *example = wb::As<IExampleComponent>(component);

        if (keyboardMonitor != nullptr && keyboardMonitor->GetKeyDown(wb::KeyCode::Space))
        {
            // Get data asset from the container
            wb::IAsset &dataAsset = assetContainer.Get(example->GetDataAssetID());
            example::IDataAsset *data = wb::As<IDataAsset>(&dataAsset);
            if (data == nullptr)
            {
                std::string err = wb::CreateErrorMessage
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"Set asset with ID ", std::to_string(example->GetDataAssetID()), " is not of type IDataAsset."}
                );

                wb::ConsoleLogErr(err);
                wb::ErrorNotify("EXAMPLE", err);
                wb::ThrowRuntimeError(err);
            }

            std::string message = wb::CreateMessage
            ({
                "Entity ID: " + std::to_string(entity->GetID()()), 
                "ExampleComponent value: " + std::to_string(example->GetValue()),
                "Data Asset ID: " + std::to_string(example->GetDataAssetID()),
                "Data : " + std::string(data->GetData()), ""
            });
            wb::ConsoleLog(message);
        }
    }
}

namespace example
{
    WB_REGISTER_SYSTEM(ExampleSystem, ExampleSystemID());
}