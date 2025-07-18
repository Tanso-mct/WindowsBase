#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/interfaces/component.h"
#include "windows_base/include/component_registry.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    const size_t &MockComponentID()
    {
        static size_t id = wb::IDFactory::CreateComponentID();
        return id;
    }

    class MockComponent : public wb::IComponent
    {
    public:
        MockComponent() = default;
        ~MockComponent() override = default;

        const size_t &GetID() const override 
        {
            return MockComponentID();
        }
    };

    WB_REGISTER_COMPONENT(MockComponent, MockComponentID());
}

TEST(ComponentRegistry, GetFactory)
{
    wb::IComponentFactory &factory = wb::gComponentRegistry.GetFactory(MockComponentID());
    EXPECT_NE(&factory, nullptr);

    std::unique_ptr<wb::IComponent> component = factory.Create();
    EXPECT_NE(component, nullptr);
    EXPECT_EQ(component->GetID(), MockComponentID());
}