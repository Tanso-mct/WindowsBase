#include "pch.h"

#include "wb_utility/include/type_cast.h"

class Base
{
public:
    virtual ~Base() = default;
};

constexpr int SAMPLE_VALUE = 7;

class Derived : public Base
{
public:
    int value = SAMPLE_VALUE;
};

TEST(TypeCast, As)
{
    Base* base = new Derived();

    {
        Derived* derived = wb::As<Derived>(base);
        EXPECT_NE(derived, nullptr);
        EXPECT_EQ(derived->value, SAMPLE_VALUE);
    }

    EXPECT_NE(base, nullptr); // Baseはまだ存在する
    delete base;
}

TEST(TypeCast, UniqueAs)
{
    std::unique_ptr<Base> base = std::make_unique<Derived>();

    {
        std::unique_ptr<Derived> derived = wb::UniqueAs<Derived>(base);
        EXPECT_NE(derived, nullptr);
        EXPECT_EQ(derived->value, SAMPLE_VALUE);
    }

    EXPECT_EQ(base, nullptr); // 所有権はDerivedに移動したため、baseはnullptrになる
}

TEST(TypeCast, CastWithReturn)
{
    std::unique_ptr<Base> base = std::make_unique<Derived>();

    {
        wb::CastWithReturn<Base, Derived> derived(base);
        EXPECT_NE(derived(), nullptr);
        EXPECT_EQ(derived()->value, SAMPLE_VALUE);
    }

    EXPECT_NE(base, nullptr); // 所有権は戻ってきているため、baseはまだ存在する
}

TEST(TypeCast, SpecifiedAsHrToStr)
{
    HRESULT hr = E_FAIL;
    std::string errMsg;
    wb::SpecifiedAs(hr, errMsg);
    
    EXPECT_FALSE(errMsg.empty());
    EXPECT_EQ(errMsg, "Unspecified error\r\n");
}