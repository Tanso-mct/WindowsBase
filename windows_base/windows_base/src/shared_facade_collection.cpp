#include "windows_base/src/pch.h"
#include "windows_base/include/shared_facade_collection.h"

void wb::SharedFacadeCollection::AddFactory(size_t id, std::unique_ptr<ISharedFacadeFactory> factory)
{
    if (sharedFacadeFactories_.find(id) != sharedFacadeFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Shared facade factory with ID ", std::to_string(id), " already exists."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // Add the factory to the collection
    sharedFacadeFactories_[id] = std::move(factory);

    // Save the key
    keys_.push_back(id);

    // Update the maximum ID if necessary
    if (id > maxId)
    {
        maxId = id;
    }
}

wb::ISharedFacadeFactory &wb::SharedFacadeCollection::GetFactory(size_t id)
{
    if (sharedFacadeFactories_.find(id) == sharedFacadeFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Shared facade factory with ID ", std::to_string(id), " does not exist."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *sharedFacadeFactories_[id];
}

size_t wb::SharedFacadeCollection::GetMaxID() const
{
    return maxId;
}

const std::vector<size_t> &wb::SharedFacadeCollection::GetKeys() const
{
    return keys_;
}

WINDOWS_BASE_API wb::SharedFacadeCollection wb::gSharedFacadeCollection;

wb::SharedFacadeRegistrar::SharedFacadeRegistrar(size_t id, std::unique_ptr<ISharedFacadeFactory> factory)
{
    gSharedFacadeCollection.AddFactory(id, std::move(factory));
}