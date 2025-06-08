#include "pch.h"

#include "wb_utility/include/console_log.h"

TEST(ConsoleLog, LogMessages)
{
    wb::ConsoleLog({"通常メッセージ", "コンソールにログを出力"});
    SUCCEED();
}

TEST(ConsoleLog, LogWarningMessages)
{
    wb::ConsoleLogWrn({"警告メッセージ", "コンソールに警告を出力"});
    SUCCEED();
}

TEST(ConsoleLog, LogErrorMessages)
{
    std::string errorMessage = wb::ConsoleLogErr
    (
        __FILE__, __LINE__, __FUNCTION__,
        {"エラーメッセージ", "コンソールにエラーを出力"}
    );
    EXPECT_FALSE(errorMessage.empty());
}