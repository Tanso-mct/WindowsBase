#include "windows_base/src/pch.h"
#include "windows_base/include/component_collection.h"

wb::ComponentCollection &wb::ComponentCollection::GetInstance()
{
    static ComponentCollection instance;
    return instance;
}

void wb::ComponentCollection::AddFactory(size_t componentID, std::unique_ptr<IComponentFactory> componentFactory)
{
    wb::ComponentCollection &instance = GetInstance();
    if (instance.componentFactories_.find(componentID) != instance.componentFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Component factory with ID ", std::to_string(componentID), " already exists."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // Add the factory to the collection
    instance.componentFactories_[componentID] = std::move(componentFactory);

    // Save the key
    instance.keys_.push_back(componentID);

    // Update the maximum ID if necessary
    if (componentID > instance.maxId)
    {
        instance.maxId = componentID;
    }
}

wb::IComponentFactory &wb::ComponentCollection::GetFactory(size_t componentID)
{
    wb::ComponentCollection &instance = GetInstance();

    if (instance.componentFactories_.find(componentID) == instance.componentFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Component factory with ID ", std::to_string(componentID), " does not exist."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *instance.componentFactories_[componentID];
}