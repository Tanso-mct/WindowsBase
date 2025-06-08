#include "pch.h"

#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

TEST(ErrorHandler, ErrorNotify) 
{
    wb::ErrorNotify("Test Error", "This is a test error message.");
}

TEST(ErrorHandler, QuitProgram) 
{
    int exitCode = 1;
    wb::QuitProgram(exitCode);
}