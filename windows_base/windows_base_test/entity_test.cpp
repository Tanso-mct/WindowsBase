#include "pch.h"

#include "windows_base/include/entity.h"
#include "windows_base/include/container_impl.h"
#include "windows_base/include/id_factory.h"
#include "windows_base/include/component_registry.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    const size_t &MockComponentID()
    {
        static size_t id = wb::IDFactory::CreateComponentID();
        return id;
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

        const size_t &GetID() const override
        {
            return MockComponentID();
        }

        int GetValue() const override
        {
            return MOCK_COMPONENT_VALUE;
        }
    };

    WB_REGISTER_COMPONENT(MockComponent, MockComponentID());
}

TEST(Entity, CreateAndDestroy)
{
    // Create containers
    std::unique_ptr<wb::IEntityContainer> entityCont = std::make_unique<wb::EntityContainer>();
    std::unique_ptr<wb::IComponentContainer> componentCont = std::make_unique<wb::ComponentContainer>();

    // Create an EntityIDView
    std::unique_ptr<wb::IEntityIDView> entityIDView = std::make_unique<wb::EntityIDView>();

    // Create an entity
    std::unique_ptr<wb::IOptionalValue> entityID = nullptr;
    {
        wb::CreatingEntity entity = wb::CreateEntity(*entityCont, *entityIDView);
        EXPECT_TRUE(entity().GetID().IsValid());
        EXPECT_TRUE(entity().GetID()() == 0); // ID should be 0 since it's the first entity created
        EXPECT_EQ(entity().GetComponentIDs().size(), 0);

        entityID = entity().GetID().Clone(); // Store the entity ID for later use

    } // If CreatingEntity goes out of scope, it will register the entity in the ID view.

    // Destroy the entity
    {
        // Get the entity using the stored ID
        wb::IEntity *entity = entityCont->PtrGet(*entityID);
        wb::DestroyEntity(entity, *entityIDView, *entityCont, *componentCont);
    }

    EXPECT_NE(entityID->IsValid(), true); // The entity ID should no longer be valid after destruction
    EXPECT_EQ(entityCont->GetSize(), 0); // The entity should be removed from the container
    EXPECT_EQ(componentCont->GetSize(), 0); // The component container should also be empty
}

TEST(Entity, AddAndGetComponent)
{
    // Create containers
    std::unique_ptr<wb::IEntityContainer> entityCont = std::make_unique<wb::EntityContainer>();
    std::unique_ptr<wb::IComponentContainer> componentCont = std::make_unique<wb::ComponentContainer>();

    // Create an EntityIDView
    std::unique_ptr<wb::IEntityIDView> entityIDView = std::make_unique<wb::EntityIDView>();

    // Create an entity
    std::unique_ptr<wb::IOptionalValue> entityID = nullptr;
    {
        wb::CreatingEntity entity = wb::CreateEntity(*entityCont, *entityIDView);
        EXPECT_TRUE(entity().GetID().IsValid());
        EXPECT_TRUE(entity().GetID()() == 0); // ID should be 0 since it's the first entity created
        EXPECT_EQ(entity().GetComponentIDs().size(), 0);

        entityID = entity().GetID().Clone(); // Store the entity ID for later use

        // Add a component to the entity
        entity().AddComponent(MockComponentID(), *componentCont);
        EXPECT_EQ(entity().GetComponentIDs().size(), 1);
        EXPECT_EQ(entity().GetComponentIDs()[0], MockComponentID());

    } // If CreatingEntity goes out of scope, it will register the entity in the ID view.

    {
        // Get the entity using the stored ID
        wb::IEntity *entity = entityCont->PtrGet(*entityID);
        ASSERT_NE(entity, nullptr); // Ensure the entity was created successfully
        EXPECT_EQ(entity->GetComponentIDs().size(), 1);
        EXPECT_EQ(entity->GetComponentIDs()[0], MockComponentID());

        // Get the component from the entity
        wb::IComponent *component = entity->GetComponent(MockComponentID(), *componentCont);
        EXPECT_NE(component, nullptr);
        EXPECT_EQ(component->GetID(), MockComponentID());

        // Verify the component value
        ::IMockComponent *mock = dynamic_cast<IMockComponent *>(component);
        ASSERT_NE(mock, nullptr);
        EXPECT_EQ(mock->GetValue(), MOCK_COMPONENT_VALUE);

        // Clean up
        wb::DestroyEntity(entity, *entityIDView, *entityCont, *componentCont);
    }

    EXPECT_NE(entityID->IsValid(), true); // The entity ID should no longer be valid after destruction
    EXPECT_EQ(entityCont->GetSize(), 0); // The entity should be removed from the container
    EXPECT_EQ(componentCont->GetSize(), 0); // The component container should also be empty
}

TEST(Entity, EntityIDView)
{
    // Create containers
    std::unique_ptr<wb::IEntityContainer> entityCont = std::make_unique<wb::EntityContainer>();
    std::unique_ptr<wb::IComponentContainer> componentCont = std::make_unique<wb::ComponentContainer>();

    // Create an EntityIDView
    std::unique_ptr<wb::IEntityIDView> entityIDView = std::make_unique<wb::EntityIDView>();

    // Create an entity
    std::unique_ptr<wb::IOptionalValue> entity1ID = nullptr;
    {
        wb::CreatingEntity entity = wb::CreateEntity(*entityCont, *entityIDView);
        EXPECT_TRUE(entity().GetID().IsValid());
        EXPECT_TRUE(entity().GetID()() == 0); // ID should be 0 since it's the first entity created
        EXPECT_EQ(entity().GetComponentIDs().size(), 0);

        entity1ID = entity().GetID().Clone(); // Store the entity ID for later use

        // Add a component to the entity
        entity().AddComponent(MockComponentID(), *componentCont);
        EXPECT_EQ(entity().GetComponentIDs().size(), 1);
        EXPECT_EQ(entity().GetComponentIDs()[0], MockComponentID());

    } // If CreatingEntity goes out of scope, it will register the entity in the ID view.

    // Create another entity
    std::unique_ptr<wb::IOptionalValue> entity2ID = nullptr;
    {
        wb::CreatingEntity entity = wb::CreateEntity(*entityCont, *entityIDView);
        EXPECT_TRUE(entity().GetID().IsValid());
        EXPECT_TRUE(entity().GetID()() == 1); // ID should be 1 since it's the second entity created
        EXPECT_EQ(entity().GetComponentIDs().size(), 0);

        entity2ID = entity().GetID().Clone(); // Store the entity ID for later use

        // Add a component to the entity
        entity().AddComponent(MockComponentID(), *componentCont);
        EXPECT_EQ(entity().GetComponentIDs().size(), 1);
        EXPECT_EQ(entity().GetComponentIDs()[0], MockComponentID());

    } // If CreatingEntity goes out of scope, it will register the entity in the ID view.

    for (const std::unique_ptr<wb::IOptionalValue> &id : (*entityIDView)(MockComponentID()))
    {
        EXPECT_TRUE(id->IsValid());

        // Get entity who has this ID
        wb::IEntity *entity = entityCont->PtrGet(*id);
        EXPECT_NE(entity, nullptr);

        // Get the component from the entity
        wb::IComponent *component = entity->GetComponent(MockComponentID(), *componentCont);
        EXPECT_NE(component, nullptr);
        EXPECT_EQ(component->GetID(), MockComponentID());

        // Verify the component value
        ::IMockComponent *mock = dynamic_cast<IMockComponent *>(component);
        ASSERT_NE(mock, nullptr);
        EXPECT_EQ(mock->GetValue(), MOCK_COMPONENT_VALUE);
    }

    // Clean up entity1
    {
        wb::IEntity *entity1 = entityCont->PtrGet(*entity1ID);
        ASSERT_NE(entity1, nullptr);

        wb::DestroyEntity(entity1, *entityIDView, *entityCont, *componentCont);
    }
    EXPECT_EQ(entityCont->GetSize(), 1); // Only entity2 should remain

    int remainingEntitiesCount = 0;
    for (const std::unique_ptr<wb::IOptionalValue> &id : (*entityIDView)(MockComponentID()))
    {
        EXPECT_TRUE(id->IsValid());

        // Get entity who has this ID
        wb::IEntity *entity = entityCont->PtrGet(*id);
        EXPECT_NE(entity, nullptr);

        // Get the component from the entity
        wb::IComponent *component = entity->GetComponent(MockComponentID(), *componentCont);
        EXPECT_NE(component, nullptr);
        EXPECT_EQ(component->GetID(), MockComponentID());

        // Verify the component value
        ::IMockComponent *mock = dynamic_cast<IMockComponent *>(component);
        ASSERT_NE(mock, nullptr);
        EXPECT_EQ(mock->GetValue(), MOCK_COMPONENT_VALUE);

        remainingEntitiesCount++;
    }

    EXPECT_EQ(remainingEntitiesCount, 1); // Only entity2 should remain

    // Clean up entity2
    {
        wb::IEntity *entity2 = entityCont->PtrGet(*entity2ID);
        ASSERT_NE(entity2, nullptr);

        wb::DestroyEntity(entity2, *entityIDView, *entityCont, *componentCont);
    }
    
    EXPECT_EQ(entityCont->GetSize(), 0); // The entity should be removed from
    EXPECT_EQ(componentCont->GetSize(), 0); // The component container should also be empty
}

