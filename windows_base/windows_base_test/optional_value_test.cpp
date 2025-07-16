#include "pch.h"

#include "windows_base/include/optional_value.h"
#pragma comment(lib, "windows_base.lib")

TEST(OptionalValue, Create)
{
    const size_t SAMPLE_VALUE = 7;
    std::shared_ptr<size_t> value = std::make_shared<size_t>(SAMPLE_VALUE);

    std::unique_ptr<wb::IOptionalValue> optionalValue = std::make_unique<wb::OptionalValue>(value);
    EXPECT_TRUE(optionalValue->IsValid());
    EXPECT_EQ((*optionalValue)(), SAMPLE_VALUE);
}

TEST(OptionalValue, Clone)
{
    const size_t SAMPLE_VALUE = 7;
    std::shared_ptr<size_t> value = std::make_shared<size_t>(SAMPLE_VALUE);

    std::unique_ptr<wb::OptionalValue> optionalValue = std::make_unique<wb::OptionalValue>(value);
    EXPECT_TRUE(optionalValue->IsValid());
    EXPECT_EQ((*optionalValue)(), SAMPLE_VALUE);

    std::unique_ptr<wb::IOptionalValue> clonedValue = optionalValue->Clone();
    EXPECT_TRUE(clonedValue->IsValid());
    EXPECT_EQ((*clonedValue)(), SAMPLE_VALUE);
}