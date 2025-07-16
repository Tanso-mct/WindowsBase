#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/shared_facade_collection.h"

namespace
{
    /*******************************************************************************************************************
     * Mock Shared Facade
    /******************************************************************************************************************/

    const size_t &MockSharedFacadeID()
    {
        static size_t id = wb::IDFactory::CreateSharedFacadeID();
        return id;
    }

    class MockSharedFacade : public wb::ISharedFacade
    {
    public:
        MockSharedFacade() = default;
        ~MockSharedFacade() override = default;

        void SetContext(std::unique_ptr<wb::IContext> context) override
        {
            // Mock implementation
        }

        bool CheckIsReady() const override
        {
            // Mock implementation
            return true;
        }
    };

    WB_REGISTER_SHARED_FACADE(MockSharedFacadeID, MockSharedFacade);
}

TEST(SharedFacadeCollection, GetFactory)
{
    wb::ISharedFacadeFactory &factory = wb::gSharedFacadeCollection.GetFactory(MockSharedFacadeID());
    EXPECT_NE(&factory, nullptr);
}