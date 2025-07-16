#include "pch.h"

#include "windows_base/include/console_log.h"
#pragma comment(lib, "windows_base.lib")

TEST(ConsoleLog, CreateMessage) 
{
    std::string result = wb::CreateMessage({"Hello", "World"});
    std::cout << result;

    int intNum = 42;
    float floatNum = 3.14f;
    double doubleNum = 2.71828;

    std::string resultWithNumbers = wb::CreateMessage
    ({
        "Hello", "World", 
        std::to_string(intNum), std::to_string(floatNum), std::to_string(doubleNum)
    });
    std::cout << resultWithNumbers;
}

TEST(ConsoleLog, CreateErrorMessage) 
{
    std::string result = wb::CreateErrorMessage
    (
        __FILE__, __LINE__, __FUNCTION__, 
        {"Error occurred", "Please check the logs"}
    );
    std::cout << result;

    int errorCode = 404;
    std::string resultWithErrorCode = wb::CreateErrorMessage
    (
        __FILE__, __LINE__, __FUNCTION__, 
        {"Error occurred", "Code: " + std::to_string(errorCode)}
    );
    std::cout << resultWithErrorCode;
}

TEST(ConsoleLog, ConsoleLog) 
{
    std::string normalMessage = "This is a normal log message.\n";
    std::string warningMessage = "This is a warning message.\n";
    std::string errorMessage = "This is an error message.\n";

    wb::ConsoleLog(normalMessage);
    wb::ConsoleLogWrn(warningMessage);
    wb::ConsoleLogErr(errorMessage);
}