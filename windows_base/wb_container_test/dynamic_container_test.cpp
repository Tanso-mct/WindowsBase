#include "pch.h"

#include "wb_container/include/dynamic_container.h"
#include "wb_container_test/dummy.h"

#include <memory>

constexpr size_t CONTAINER_INITIAL_SIZE = 5;

TEST(DynamicContainer, Create)
{
    // DynamicContainerのインスタンスを作成
    std::unique_ptr<wb::DynamicContainer<Dummy>> container 
    = std::make_unique<wb::DynamicContainer<Dummy>>();

    // コンテナを作成
    container->Create(CONTAINER_INITIAL_SIZE);

    // 再度作成。エラーが発生することを確認
    // container->Create(5);

    // コンテナのサイズを確認
    EXPECT_EQ(container->GetSize(), CONTAINER_INITIAL_SIZE);
}

TEST(DynamicContainer, Clear)
{
    std::unique_ptr<wb::DynamicContainer<Dummy>> container
    = std::make_unique<wb::DynamicContainer<Dummy>>();

    container->Create(CONTAINER_INITIAL_SIZE);

    // コンテナをクリア
    container->Clear();

    // コンテナのサイズを確認
    EXPECT_EQ(container->GetSize(), 0);
}

TEST(DynamicContainer, SetAndGet)
{
    std::unique_ptr<wb::DynamicContainer<Dummy>> container
    = std::make_unique<wb::DynamicContainer<Dummy>>();

    container->Create(CONTAINER_INITIAL_SIZE);

    // ダミーの作成と、コンテナへのセット
    const int DUMMY0_VALUE = 100;
    {
        std::unique_ptr<Dummy> dummy = std::make_unique<Dummy>(DUMMY0_VALUE);
        container->Set(0, std::move(dummy));
    }

    const int DUMMY1_VALUE = 200;
    {
        std::unique_ptr<Dummy> dummy = std::make_unique<Dummy>(DUMMY1_VALUE);
        container->Set(1, std::move(dummy));
    }

    // コンテナからダミーを取得し、値を確認
    {
        std::unique_ptr<Dummy>& dummy = container->Get(0);
        EXPECT_EQ(dummy->GetValue(), DUMMY0_VALUE);
    }

    {
        std::unique_ptr<Dummy>& dummy = container->Get(1);
        EXPECT_EQ(dummy->GetValue(), DUMMY1_VALUE);
    }
}

TEST(DynamicContainer, Add)
{
    std::unique_ptr<wb::DynamicContainer<Dummy>> container
    = std::make_unique<wb::DynamicContainer<Dummy>>();

    container->Create(CONTAINER_INITIAL_SIZE);

    // ダミーの作成と、コンテナへの追加
    const int DUMMY0_VALUE = 100;
    size_t dummy0Index = 0;
    {
        std::unique_ptr<Dummy> dummy = std::make_unique<Dummy>(DUMMY0_VALUE);
        dummy0Index = container->Add(std::move(dummy));
        EXPECT_EQ(dummy0Index, CONTAINER_INITIAL_SIZE); // 最初の要素はコンテナの初期サイズと同じ
    }

    const int DUMMY1_VALUE = 200;
    size_t dummy1Index = 0;
    {
        std::unique_ptr<Dummy> dummy = std::make_unique<Dummy>(DUMMY1_VALUE);
        dummy1Index = container->Add(std::move(dummy));
        EXPECT_EQ(dummy1Index, CONTAINER_INITIAL_SIZE + 1); // 2番目の要素は初期サイズ+1
    }

    // コンテナからダミーを取得し、値を確認
    {
        std::unique_ptr<Dummy>& dummy = container->Get(dummy0Index);
        EXPECT_EQ(dummy->GetValue(), DUMMY0_VALUE);
    }

    {
        std::unique_ptr<Dummy>& dummy = container->Get(dummy1Index);
        EXPECT_EQ(dummy->GetValue(), DUMMY1_VALUE);
    }
}

TEST(DynamicContainer, Release)
{
    std::unique_ptr<wb::DynamicContainer<Dummy>> container
    = std::make_unique<wb::DynamicContainer<Dummy>>();

    container->Create(CONTAINER_INITIAL_SIZE);

    // ダミーの作成と、コンテナへの追加
    const int DUMMY0_VALUE = 100;
    {
        std::unique_ptr<Dummy> dummy = std::make_unique<Dummy>(DUMMY0_VALUE);
        container->Set(static_cast<size_t>(DummyID::ID0), std::move(dummy));
    }

    // ダミーをリリース
    {
        std::unique_ptr<Dummy> releasedDummy = container->Release(static_cast<size_t>(DummyID::ID0));
        EXPECT_EQ(releasedDummy->GetValue(), DUMMY0_VALUE);
    }

    // リリース後、コンテナからダミーを取得し、nullptrであることを確認
    {
        std::unique_ptr<Dummy>& dummy = container->Get(static_cast<size_t>(DummyID::ID0));
        EXPECT_EQ(dummy, nullptr);
    }
}

