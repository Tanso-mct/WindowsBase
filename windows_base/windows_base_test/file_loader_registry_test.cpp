#include "pch.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/file_loader_registry.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    const size_t &MockFileLoaderID()
    {
        static size_t id = wb::IDFactory::CreateFileLoaderID();
        return id;
    }

    class MockFileLoader : public wb::IFileLoader
    {
    public:
        MockFileLoader() = default;
        ~MockFileLoader() override = default;

        const size_t &GetID() const override
        {
            return MockFileLoaderID();
        }

        std::unique_ptr<wb::IFileData> Load(std::string_view path) override
        {
            // Mock implementation, returning nullptr for simplicity
            return nullptr;
        }
    };

    WB_REGISTER_FILE_LOADER(MockFileLoaderID(), MockFileLoader);
}

TEST(FileLoaderRegistry, GetLoader)
{
    wb::IFileLoader &loader = wb::gFileLoaderRegistry.GetLoader(MockFileLoaderID());
    EXPECT_NE(&loader, nullptr);

    EXPECT_EQ(loader.GetID(), MockFileLoaderID());

    std::unique_ptr<wb::IFileData> fileData = loader.Load("mock_path");
    EXPECT_EQ(fileData, nullptr); // Since Load returns nullptr in the mock
}