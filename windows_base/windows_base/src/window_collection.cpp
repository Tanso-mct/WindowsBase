#include "windows_base/src/pch.h"
#include "windows_base/include/window_collection.h"

void wb::WindowCollection::AddFactories(size_t id, std::unique_ptr<IWindowFacadeFactory> facadeFactory, std::unique_ptr<IWindowEventFactory> eventFactory)
{
    if (facadeFactories_.find(id) != facadeFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Window with ID ", std::to_string(id), " already exists."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // Add the factories
    facadeFactories_[id] = std::move(facadeFactory);
    eventFactories_[id] = std::move(eventFactory);

    // Save the key
    keys_.push_back(id);

    // Update the maximum ID if necessary
    if (id > maxId_)
    {
        maxId_ = id;
    }
}

wb::IWindowFacadeFactory &wb::WindowCollection::GetFacadeFactory(size_t id)
{
    if (facadeFactories_.find(id) == facadeFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Window facade factory for ID ", std::to_string(id), " does not exist."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *facadeFactories_.at(id);
}

wb::IWindowEventFactory &wb::WindowCollection::GetEventFactory(size_t id)
{
    if (eventFactories_.find(id) == eventFactories_.end())
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Window event factory for ID ", std::to_string(id), " does not exist."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *eventFactories_.at(id);
}

size_t wb::WindowCollection::GetMaxID() const
{
    return maxId_;
}

const std::vector<size_t> &wb::WindowCollection::GetKeys() const
{
    return keys_;
}

WINDOWS_BASE_API wb::WindowCollection wb::gWindowCollection;

wb::WindowRegistrar::WindowRegistrar
(
    size_t windowID,
    std::unique_ptr<IWindowFacadeFactory> facadeFactory, std::unique_ptr<IWindowEventFactory> eventFactory
){
    wb::gWindowCollection.AddFactories(windowID, std::move(facadeFactory), std::move(eventFactory));
}