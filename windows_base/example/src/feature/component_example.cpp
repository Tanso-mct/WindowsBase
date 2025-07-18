#include "example/src/pch.h"
#include "example/include/feature/component_example.h"

const size_t &example::ExampleComponentID()
{
    static size_t id = wb::IDFactory::CreateComponentID();
    return id;
}

const size_t &example::ExampleComponent::GetID() const
{
    return ExampleComponentID();
}

namespace example
{
    WB_REGISTER_COMPONENT(ExampleComponent, ExampleComponentID());
}