#include "windows_base/src/pch.h"
#include "windows_base/include/component_collection.h"

wb::ComponentCollection &wb::ComponentCollection::GetInstance()
{
    static ComponentCollection instance;
    return instance;
}

void wb::ComponentCollection::AddFactory(size_t componentID, std::unique_ptr<IComponentFactory> componentFactory)
{
}

wb::IComponentFactory &wb::ComponentCollection::GetFactory(size_t componentID)
{
    return *componentFactories_[componentID];
}