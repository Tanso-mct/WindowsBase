#include "wb_monitor/src/pch.h"
#include "wb_monitor/include/keyboard_monitor.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

void wb::KeyboardMonitor::UpdateState()
{
    for (size_t i = 0; i < static_cast<size_t>(KeyCode::Size); ++i)
    {
        // キーボードが押された後の場合
        if (keyboardContext_->KeyStates()[i] == INPUT_DOWN)
        {
            keyboardContext_->KeyStates()[i] = INPUT_PRESSED; // 押されている状態に変更
        }

        // キーボードが離された後の場合
        if (keyboardContext_->KeyStates()[i] == INPUT_UP)
        {
            keyboardContext_->KeyStates()[i] = INPUT_NONE; // 離されていない状態に変更

            // 前回のキーとして設定
            keyboardContext_->PreviousKey() = static_cast<KeyCode>(i);

            // 前回のキーが押された時間を更新
            keyboardContext_->PreviousKeyTime() = std::chrono::high_resolution_clock::now();
        }
    }
}

void wb::KeyboardMonitor::SetUpContext(std::unique_ptr<IKeyboardContext> keyContext)
{
    // 配列のサイズをキーボードのキーコードの数に合わせて初期化
    keyContext->KeyStates().resize(static_cast<size_t>(KeyCode::Size), wb::INPUT_NONE);

    // コンテキストを設定
    keyboardContext_ = std::move(keyContext);
}

void wb::KeyboardMonitor::SetUpCodeTable(std::unique_ptr<IKeyCodeTable> keyCodeTable)
{
    // キーコードテーブルのセットアップ
    keyCodeTable->SetUpTable();
    if (!keyCodeTable->IsSetUp())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"キーコードテーブルのセットアップが完了していません。"}
        );
        wb::ErrorNotify("KEYBOARD_MONITOR", err);
        wb::QuitProgram();
    }

    // キーコードテーブルを設定
    keyCodeTable_ = std::move(keyCodeTable);
}

bool wb::KeyboardMonitor::IsSetUp() const
{
    if (keyboardContext_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"キーボードコンテキストが設定されていません。"}
        );
        wb::ErrorNotify("KEYBOARD_MONITOR", err);
        return false;
    }

    if (keyCodeTable_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"キーコードテーブルが設定されていません。"}
        );
        wb::ErrorNotify("KEYBOARD_MONITOR", err);
        return false;
    }

    return true;
}

void wb::KeyboardMonitor::EditStateByDown(WPARAM wParam, LPARAM lParam)
{
    // キーコードを取得
    KeyCode keyCode = keyCodeTable_->GetKeyCode(wParam, lParam);
    if (keyCode == KeyCode::Null) return; // 対応するキーでない場合、何もしない

    // キーの状態を更新
    keyboardContext_->KeyStates()[static_cast<size_t>(keyCode)] = INPUT_DOWN;
}

void wb::KeyboardMonitor::EditStateByUp(WPARAM wParam, LPARAM lParam)
{
    // キーコードを取得
    KeyCode keyCode = keyCodeTable_->GetKeyCode(wParam, lParam);
    if (keyCode == KeyCode::Null) return; // 対応するキーでない場合、何もしない

    // キーの状態を更新
    keyboardContext_->KeyStates()[static_cast<size_t>(keyCode)] = INPUT_UP;
}

bool wb::KeyboardMonitor::GetKey(const KeyCode &keyCode)
{
    if (keyCode == KeyCode::Null || keyCode >= KeyCode::Size)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"無効なキーコードが指定されました。"}
        );
        wb::ErrorNotify("KEYBOARD_MONITOR", err);
        return false; // 無効なキーコードの場合、falseを返す
    }

    if 
    (
        keyboardContext_->KeyStates()[static_cast<size_t>(keyCode)] == INPUT_DOWN ||
        keyboardContext_->KeyStates()[static_cast<size_t>(keyCode)] == INPUT_PRESSED
    ){
        return true; // キーが押されている状態
    }

    return false; // キーが押されていない状態
}

bool wb::KeyboardMonitor::GetKeyDown(const KeyCode &keyCode)
{
    if (keyCode == KeyCode::Null || keyCode >= KeyCode::Size)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"無効なキーコードが指定されました。"}
        );
        wb::ErrorNotify("KEYBOARD_MONITOR", err);
        return false; // 無効なキーコードの場合、falseを返す
    }

    if (keyboardContext_->KeyStates()[static_cast<size_t>(keyCode)] == INPUT_DOWN)
    {
        return true; // キーが押された状態
    }

    return false; // キーが押されていない状態
}

bool wb::KeyboardMonitor::GetKeyUp(const KeyCode &keyCode)
{
    if (keyCode == KeyCode::Null || keyCode >= KeyCode::Size)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"無効なキーコードが指定されました。"}
        );
        wb::ErrorNotify("KEYBOARD_MONITOR", err);
        return false; // 無効なキーコードの場合、falseを返す
    }

    if (keyboardContext_->KeyStates()[static_cast<size_t>(keyCode)] == INPUT_UP)
    {
        return true; // キーが離された状態
    }

    return false; // キーが離されていない状態
}

bool wb::KeyboardMonitor::GetKeyDoubleTap(const KeyCode &keyCode, const double &threshold)
{
    if (keyCode == KeyCode::Null || keyCode >= KeyCode::Size)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"無効なキーコードが指定されました。"}
        );
        wb::ErrorNotify("KEYBOARD_MONITOR", err);
        return false; // 無効なキーコードの場合、falseを返す
    }

    // ダブルタップは離された状態でのみチェック
    if (keyboardContext_->KeyStates()[static_cast<size_t>(keyCode)] != INPUT_UP) return false;

    // 前回のキーが同じで、かつ、前回のキーが押された時間からの経過時間が閾値以下の場合
    if 
    (
        keyboardContext_->PreviousKey() == keyCode &&
        std::chrono::duration<double>
        (
            std::chrono::high_resolution_clock::now() - keyboardContext_->PreviousKeyTime()
        ).count() <= threshold
    ){
        return true; // ダブルタップと判断
    }

    return false; // ダブルタップではない
}