#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/system_collection.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    size_t MockSystemID()
    {
        static size_t mockSystemID = wb::IDFactory::CreateSystemID();
        return mockSystemID;
    }

    class MockSystem : public wb::ISystem
    {
    public:
        MockSystem() = default;
        ~MockSystem() override = default;

        size_t GetID() const override
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
    
    // Call the Update method to ensure the system is functional
    wb::SystemArgument args;
    system->Update(args);
}