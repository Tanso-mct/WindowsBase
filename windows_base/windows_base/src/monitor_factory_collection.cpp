#include "windows_base/src/pch.h"
#include "windows_base/include/monitor_factory_collection.h"

void wb::MonitorFactoryCollection::AddFactory(size_t monitorID, std::unique_ptr<IMonitorFactory> monitorFactory)
{
    if (monitorFactories_.find(monitorID) != monitorFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Monitor factory with ID ", std::to_string(monitorID), " already exists."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // Add the factory to the collection
    monitorFactories_[monitorID] = std::move(monitorFactory);

    // Save the key
    keys_.push_back(monitorID);

    // Update the maximum ID if necessary
    if (monitorID > maxId)
    {
        maxId = monitorID;
    }
}

wb::IMonitorFactory &wb::MonitorFactoryCollection::GetFactory(size_t monitorID)
{
    if (monitorFactories_.find(monitorID) == monitorFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Monitor factory with ID ", std::to_string(monitorID), " does not exist."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *monitorFactories_[monitorID];
}

size_t wb::MonitorFactoryCollection::GetMaxID() const
{
    return maxId;
}

const std::vector<size_t> &wb::MonitorFactoryCollection::GetKeys() const
{
    return keys_;
}

WINDOWS_BASE_API wb::MonitorFactoryCollection wb::gMonitorFactoryCollection;

wb::MonitorFactoryRegistrar::MonitorFactoryRegistrar(size_t monitorID, std::unique_ptr<IMonitorFactory> monitorFactory)
{
    gMonitorFactoryCollection.AddFactory(monitorID, std::move(monitorFactory));
}