#include "windows_base/src/pch.h"
#include "windows_base/include/monitor_collection.h"

void wb::MonitorCollection::Add(size_t id, size_t factoryID)
{
    if (factoryIDs_.find(id) != factoryIDs_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Monitor with ID ", std::to_string(id), " already exists."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // Add the factory ID to the collection
    factoryIDs_[id] = factoryID;

    // Save the key
    keys_.push_back(id);

    // Update the maximum ID if necessary
    if (id > maxId)
    {
        maxId = id;
    }
}

const size_t &wb::MonitorCollection::GetFactoryID(size_t id) const
{
    if (factoryIDs_.find(id) == factoryIDs_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Monitor Factory for ID ", std::to_string(id), " does not exist."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return factoryIDs_.at(id);
}

size_t wb::MonitorCollection::GetMaxID() const
{
    return maxId;
}

const std::vector<size_t> &wb::MonitorCollection::GetKeys() const
{
    return keys_;
}

WINDOWS_BASE_API wb::MonitorCollection wb::gMonitorCollection;

wb::MonitorRegistrar::MonitorRegistrar(size_t monitorID, size_t factoryID)
{
    wb::gMonitorCollection.Add(monitorID, factoryID);
}