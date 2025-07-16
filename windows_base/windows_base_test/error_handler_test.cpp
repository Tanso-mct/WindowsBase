#include "pch.h"

#include "windows_base/include/error_handler.h"
#pragma comment(lib, "windows_base.lib")

TEST(ErrorHandler, ErrorNotify)
{
    wb::ErrorNotify("Test Title", "Test Message");
}

TEST(ErrorHandler, ThrowRuntimeError)
{
    EXPECT_THROW({wb::ThrowRuntimeError("Test Runtime Error");}, std::runtime_error);
}