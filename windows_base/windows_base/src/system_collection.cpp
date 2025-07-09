#include "windows_base/src/pch.h"
#include "windows_base/include/system_collection.h"

wb::SystemCollection &wb::SystemCollection::GetInstance()
{
    static SystemCollection instance;
    return instance;
}

void wb::SystemCollection::AddFactory(size_t systemID, std::unique_ptr<ISystemFactory> systemFactory)
{
    wb::SystemCollection &instance = GetInstance();
    if (instance.systemFactories_.find(systemID) != instance.systemFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"System factory with ID ", std::to_string(systemID), " already exists."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // Add the factory to the collection
    instance.systemFactories_[systemID] = std::move(systemFactory);

    // Save the key
    instance.keys_.push_back(systemID);

    // Update the maximum ID if necessary
    if (systemID > instance.maxId)
    {
        instance.maxId = systemID;
    }
}

wb::ISystemFactory &wb::SystemCollection::GetFactory(size_t systemID)
{
    wb::SystemCollection &instance = GetInstance();

    if (instance.systemFactories_.find(systemID) == instance.systemFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"System factory with ID ", std::to_string(systemID), " does not exist."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *instance.systemFactories_[systemID];
}