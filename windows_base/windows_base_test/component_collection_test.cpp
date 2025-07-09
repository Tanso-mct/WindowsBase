#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/component_collection.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    size_t MockComponentID()
    {
        static size_t mockComponentID = wb::IDFactory::CreateComponentID();
        return mockComponentID;
    }

    class MockComponent : public wb::IComponent
    {
    public:
        MockComponent() = default;
        ~MockComponent() override = default;

        size_t GetID() const override 
        {
            return MockComponentID();
        }
    };

    WB_REGISTER_COMPONENT(MockComponent, MockComponentID());
}

TEST(ComponentCollection, GetFactory)
{
    wb::ComponentCollection collection;

    wb::IComponentFactory &factory = collection.GetFactory(MockComponentID());
    EXPECT_NE(&factory, nullptr);

    std::unique_ptr<wb::IComponent> component = factory.Create();
    EXPECT_NE(component, nullptr);
    EXPECT_EQ(component->GetID(), MockComponentID());
}