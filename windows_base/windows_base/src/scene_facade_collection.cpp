#include "windows_base/src/pch.h"
#include "windows_base/include/scene_facade_collection.h"

void wb::SceneFacadeCollection::AddFactory(size_t id, std::unique_ptr<ISceneFacadeFactory> factory)
{
    if (sceneFacadeFactories_.find(id) != sceneFacadeFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Scene facade factory with ID ", std::to_string(id), " already exists."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // Add the factory to the collection
    sceneFacadeFactories_[id] = std::move(factory);

    // Save the key
    keys_.push_back(id);

    // Update the maximum ID if necessary
    if (id > maxId)
    {
        maxId = id;
    }
}

wb::ISceneFacadeFactory &wb::SceneFacadeCollection::GetFactory(size_t id)
{
    if (sceneFacadeFactories_.find(id) == sceneFacadeFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Scene facade factory with ID ", std::to_string(id), " does not exist."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *sceneFacadeFactories_[id];
}

size_t wb::SceneFacadeCollection::GetMaxID() const
{
    return maxId;
}

const std::vector<size_t> &wb::SceneFacadeCollection::GetKeys() const
{
    return keys_;
}

WINDOWS_BASE_API wb::SceneFacadeCollection wb::gSceneFacadeCollection;

wb::SceneFacadeRegistrar::SceneFacadeRegistrar(size_t id, std::unique_ptr<ISceneFacadeFactory> factory)
{
    wb::gSceneFacadeCollection.AddFactory(id, std::move(factory));
}