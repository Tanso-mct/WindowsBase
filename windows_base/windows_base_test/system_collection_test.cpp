#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/interfaces/system.h"
#include "windows_base/include/system.h"
#include "windows_base/include/system_collection.h"
#include "windows_base/include/entity.h"
#include "windows_base/include/container_impl.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    const size_t &MockSystemID()
    {
        static size_t id = wb::IDFactory::CreateSystemID();
        return id;
    }

    class MockSystem : public wb::ISystem
    {
    public:
        MockSystem() = default;
        ~MockSystem() override = default;

        const size_t &GetID() const override
        {
            return MockSystemID();
        }

        void Update(const wb::SystemArgument &args) override
        {
            // Mock update logic
            std::cout << "MockSystem Update called." << std::endl;
        }
    };

    WB_REGISTER_SYSTEM(::MockSystem, ::MockSystemID());
}

TEST(SystemCollection, GetFactory)
{
    wb::SystemCollection &collection = wb::GetSystemCollectionInstance();

    wb::ISystemFactory &factory = collection.GetFactory(MockSystemID());
    EXPECT_NE(&factory, nullptr);

    std::unique_ptr<wb::ISystem> system = factory.Create();
    EXPECT_NE(system, nullptr);

    std::unique_ptr<wb::IEntityContainer> entityCont = std::make_unique<wb::EntityContainer>();
    std::unique_ptr<wb::IComponentContainer> componentCont = std::make_unique<wb::ComponentContainer>();
    std::unique_ptr<wb::IEntityIDView> entityIDView = std::make_unique<wb::EntityIDView>();
    
    // Call the Update method to ensure the system is functional
    wb::SystemArgument args
    (
        *entityCont,
        *componentCont,
        *entityIDView
    );
    system->Update(args);
}