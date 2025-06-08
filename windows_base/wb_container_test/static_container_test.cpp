#include "pch.h"

#include "wb_container/include/static_container.h"
#include "wb_container_test/dummy.h"

#include <memory>

TEST(StaticContainer, Create)
{
    // StaticContainerのインスタンスを作成
    std::unique_ptr<wb::StaticContainer<Dummy, DummyID>> container 
    = std::make_unique<wb::StaticContainer<Dummy, DummyID>>();

    // コンテナを作成
    container->Create(static_cast<size_t>(DummyID::Size));

    // 再度作成。エラーが発生することを確認
    // container->Create(5);

    // コンテナのサイズを確認
    EXPECT_EQ(container->GetSize(), static_cast<size_t>(DummyID::Size));
}

TEST(StaticContainer, Clear)
{
    std::unique_ptr<wb::StaticContainer<Dummy, DummyID>> container
    = std::make_unique<wb::StaticContainer<Dummy, DummyID>>();

    container->Create(static_cast<size_t>(DummyID::Size));

    // コンテナをクリア
    container->Clear();

    // コンテナのサイズを確認
    EXPECT_EQ(container->GetSize(), 0);
}

TEST(StaticContainer, SetAndGet)
{
    std::unique_ptr<wb::StaticContainer<Dummy, DummyID>> container
    = std::make_unique<wb::StaticContainer<Dummy, DummyID>>();

    container->Create(static_cast<size_t>(DummyID::Size));

    // ダミーの作成と、コンテナへのセット
    const int DUMMY0_VALUE = 100;
    {
        std::unique_ptr<Dummy> dummy = std::make_unique<Dummy>(DUMMY0_VALUE);
        container->Set(DummyID::ID0, std::move(dummy));
    }

    const int DUMMY1_VALUE = 200;
    {
        std::unique_ptr<Dummy> dummy = std::make_unique<Dummy>(DUMMY1_VALUE);
        container->Set(DummyID::ID1, std::move(dummy));
    }

    // コンテナからダミーを取得し、値を確認
    {
        std::unique_ptr<Dummy>& dummy = container->Get(DummyID::ID0);
        EXPECT_EQ(dummy->GetValue(), DUMMY0_VALUE);
    }

    {
        std::unique_ptr<Dummy>& dummy = container->Get(DummyID::ID1);
        EXPECT_EQ(dummy->GetValue(), DUMMY1_VALUE);
    }
}

TEST(StaticContainer, Release)
{
    std::unique_ptr<wb::StaticContainer<Dummy, DummyID>> container
    = std::make_unique<wb::StaticContainer<Dummy, DummyID>>();

    container->Create(static_cast<size_t>(DummyID::Size));

    // ダミーの作成と、コンテナへのセット
    const int DUMMY0_VALUE = 100;
    {
        std::unique_ptr<Dummy> dummy = std::make_unique<Dummy>(DUMMY0_VALUE);
        container->Set(DummyID::ID0, std::move(dummy));
    }

    // コンテナからダミーをリリースし、値を確認
    {
        std::unique_ptr<Dummy> releasedDummy = container->Release(DummyID::ID0);
        EXPECT_EQ(releasedDummy->GetValue(), DUMMY0_VALUE);
    }

    // リリース後、コンテナからダミーを取得し、nullptrであることを確認
    {
        std::unique_ptr<Dummy>& dummy = container->Get(DummyID::ID0);
        EXPECT_EQ(dummy, nullptr);
    }
}