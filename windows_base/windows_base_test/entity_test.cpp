#include "pch.h"

#include "windows_base/include/entity.h"
#include "windows_base/include/container_impl.h"
#include "windows_base/include/id_factory.h"
#include "windows_base/include/component.h"
#include "windows_base/include/component_collection.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    size_t MockComponentID()
    {
        static size_t mockComponentID = wb::IDFactory::CreateComponentID();
        return mockComponentID;
    }

    class IMockComponent : public wb::IComponent
    {
    public:
        virtual ~IMockComponent() = default;
        virtual int GetValue() const = 0;
    };

    constexpr int MOCK_COMPONENT_VALUE = 7;

    class MockComponent : public IMockComponent
    {
    public:
        MockComponent() = default;
        ~MockComponent() override = default;

        size_t GetID() const override
        {
            return MockComponentID();
        }

        int GetValue() const override
        {
            return MOCK_COMPONENT_VALUE;
        }
    };

    WB_REGISTER_COMPONENT(::MockComponent, ::MockComponentID());
}

TEST(Entity, CreateAndDestroy)
{
    // Create containers
    std::unique_ptr<wb::IEntityContainer> entityCont = std::make_unique<wb::EntityContainer>();
    std::unique_ptr<wb::IComponentContainer> componentCont = std::make_unique<wb::ComponentContainer>();

    // Create an EntityIDView
    wb::EntityIDView entityIDView;

    // Create an entity
    wb::IEntity &entity = wb::CreateEntity(*entityCont, entityIDView);
    EXPECT_TRUE(entity.GetID().IsValid());
    EXPECT_TRUE(entity.GetID()() == 0); // ID should be 0 since it's the first entity created
    EXPECT_EQ(entity.GetComponentIDs().size(), 0);

    // Destroy the entity
    wb::DestroyEntity(&entity, entityIDView, *entityCont, *componentCont);
    EXPECT_EQ(entityCont->GetSize(), 0); // The entity should be removed from the container
    EXPECT_EQ(componentCont->GetSize(), 0); // The component container should also be empty
}

