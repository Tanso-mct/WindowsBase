#include "pch.h"

#include "windows_base/include/type_cast.h"
#pragma comment(lib, "windows_base.lib")

TEST(TypeCast, As)
{
    class Base
    {
    public:
        virtual ~Base() = default;
    };

    class Derived : public Base
    {
    public:
        int value = 42;
    };

    Base* basePtr = new Derived();
    Derived* derivedPtr = wb::As<Derived>(basePtr);
    
    EXPECT_NE(derivedPtr, nullptr);
    EXPECT_EQ(derivedPtr->value, 42);

    delete basePtr; // Clean up
}

TEST(TypeCast, UniqueAs)
{
    class Base
    {
    public:
        virtual ~Base() = default;
    };

    class Derived : public Base
    {
    public:
        int value = 42;
    };

    std::unique_ptr<Base> basePtr = std::make_unique<Derived>();
    std::unique_ptr<Derived> derivedPtr = wb::UniqueAs<Derived>(basePtr);

    EXPECT_NE(derivedPtr, nullptr);
    EXPECT_EQ(derivedPtr->value, 42);
    EXPECT_EQ(basePtr, nullptr); // Ownership should be transferred
}

TEST(TypeCast, CastWithReturn)
{
    class Base
    {
    public:
        virtual ~Base() = default;
    };

    class Derived : public Base
    {
    public:
        int value = 42;
    };

    std::unique_ptr<Base> basePtr = std::make_unique<Derived>();
    
    {
        wb::CastWithReturn<Base, Derived> cast(basePtr);
        EXPECT_NE(cast(), nullptr);
        EXPECT_EQ(cast()->value, 42);
    } // Ownership should return to basePtr

    EXPECT_NE(basePtr, nullptr); // Ownership should be restored
}

TEST(TypeCast, SpecifiedAs)
{
    HRESULT hr = E_FAIL; // Example HRESULT
    std::string errorMessage;
    
    wb::SpecifiedAs(hr, errorMessage);
    
    EXPECT_EQ(errorMessage, "Unspecified error\r\n");
}