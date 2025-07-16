#include "windows_base/src/pch.h"
#include "windows_base/include/system_collection.h"

void wb::SystemCollection::AddFactory(size_t systemID, std::unique_ptr<ISystemFactory> systemFactory)
{
    if (systemFactories_.find(systemID) != systemFactories_.end())
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
    systemFactories_[systemID] = std::move(systemFactory);

    // Save the key
    keys_.push_back(systemID);

    // Update the maximum ID if necessary
    if (systemID > maxId)
    {
        maxId = systemID;
    }
}

wb::ISystemFactory &wb::SystemCollection::GetFactory(size_t systemID)
{
    if (systemFactories_.find(systemID) == systemFactories_.end())
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

    return *systemFactories_[systemID];
}

size_t wb::SystemCollection::GetMaxID() const
{
    return maxId;
}

const std::vector<size_t> &wb::SystemCollection::GetKeys() const
{
    return keys_;
}

WINDOWS_BASE_API wb::SystemCollection wb::gSystemCollection;

wb::SystemRegistrar::SystemRegistrar(size_t systemID, std::unique_ptr<ISystemFactory> systemFactory)
{
    gSystemCollection.AddFactory(systemID, std::move(systemFactory));
}

