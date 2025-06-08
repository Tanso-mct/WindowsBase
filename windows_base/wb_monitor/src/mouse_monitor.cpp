#include "wb_monitor/src/pch.h"
#include "wb_monitor/include/mouse_monitor.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

void wb::MouseMonitor::UpdateState()
{
    for (size_t i = 0; i < static_cast<size_t>(MouseCode::Size); ++i)
    {
        // マウスボタンが押された後の場合
        if (mouseContext_->ButtonStates()[i] == INPUT_DOWN)
        {
            mouseContext_->ButtonStates()[i] = INPUT_PRESSED; // 押されている状態に変更
        }

        // マウスボタンが離された後の場合
        if (mouseContext_->ButtonStates()[i] == INPUT_UP)
        {
            mouseContext_->ButtonStates()[i] = INPUT_NONE; // 離されていない状態に変更

            // 前回のボタンとして設定
            mouseContext_->PreviousButton() = static_cast<MouseCode>(i);

            // 前回のボタンが押された時間を更新
            mouseContext_->PreviousButtonTime() = std::chrono::high_resolution_clock::now();
        }
    }

    // スクロールデルタをリセット
    mouseContext_->ScrollDelta() = 0;
}

void wb::MouseMonitor::SetUpContext(std::unique_ptr<IMouseContext> context)
{
    // 配列のサイズをマウスのボタンコードの数に合わせて初期化
    context->ButtonStates().resize(static_cast<size_t>(MouseCode::Size), wb::INPUT_NONE);

    // コンテキストの設定
    mouseContext_ = std::move(context);
}

void wb::MouseMonitor::SetUpCodeTable(std::unique_ptr<IMouseCodeTable> mouseCodeTable)
{
    // マウスコードテーブルのセットアップ
    mouseCodeTable->SetUpTable();
    if (!mouseCodeTable->IsSetUp())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"MouseCodeTableのセットアップが完了していません。"}
        );
        wb::ErrorNotify("MOUSE_MONITOR", err);
        wb::QuitProgram();
    }

    // マウスコードテーブルの設定
    mouseCodeTable_ = std::move(mouseCodeTable);
}

void wb::MouseMonitor::SetUpInputTypeTable(std::unique_ptr<IInputTypeTable> inputTypeTable)
{
    // 入力タイプテーブルのセットアップ
    inputTypeTable->SetUpTable();
    if (!inputTypeTable->IsSetUp())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"InputTypeTableのセットアップが完了していません。"}
        );
        wb::ErrorNotify("MOUSE_MONITOR", err);
        wb::QuitProgram();
    }

    // 入力タイプテーブルの設定
    inputTypeTable_ = std::move(inputTypeTable);
}

bool wb::MouseMonitor::IsSetUp() const
{
    if (mouseContext_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"MouseContextがセットアップされていません。"}
        );
        wb::ErrorNotify("MOUSE_MONITOR", err);
        return false;
    }

    if (mouseCodeTable_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"MouseCodeTableがセットアップされていません。"}
        );
        wb::ErrorNotify("MOUSE_MONITOR", err);
        return false;
    }

    if (inputTypeTable_ == nullptr)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"InputTypeTableがセットアップされていません。"}
        );
        wb::ErrorNotify("MOUSE_MONITOR", err);
        return false;
    }

    return true;
}

void wb::MouseMonitor::EditState(UINT msg, WPARAM wParam, LPARAM lParam)
{
    // WM_MOUSEWHEELの場合
    if (msg == WM_MOUSEWHEEL)
    {
        // スクロールデルタを更新
        mouseContext_->ScrollDelta() = GET_WHEEL_DELTA_WPARAM(wParam);
        return;
    }

    // WM_MOUSEMOVEの場合
    if (msg == WM_MOUSEMOVE)
    {
        // マウスの位置を更新
        mouseContext_->PositionX() = LOWORD(lParam);
        mouseContext_->PositionY() = HIWORD(lParam);
        return;
    }

    // マウスコードを取得
    wb::MouseCode mouseCode = mouseCodeTable_->GetMouseCode(msg, wParam);
    if (mouseCode == wb::MouseCode::Null) return; // 無効なマウスコードは無視

    // 入力タイプを取得
    UINT inputType = inputTypeTable_->GetInputType(msg);
    if (inputType == wb::INPUT_NONE) return; // 無効な入力タイプは無視

    // マウスコンテキストの状態を更新
    mouseContext_->ButtonStates()[static_cast<size_t>(mouseCode)] = inputType;
}

bool wb::MouseMonitor::GetButton(const MouseCode &mouseCode)
{
    if (mouseCode == MouseCode::Null || mouseCode >= MouseCode::Size)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"無効なマウスコードが指定されました。"}
        );
        wb::ErrorNotify("MOUSE_MONITOR", err);
        return false;
    }

    if 
    (
        mouseContext_->ButtonStates()[static_cast<size_t>(mouseCode)] == wb::INPUT_DOWN ||
        mouseContext_->ButtonStates()[static_cast<size_t>(mouseCode)] == wb::INPUT_PRESSED
    ){
        return true; // ボタンが押されている場合、trueを返す
    }

    return false; // ボタンが押されていない場合、falseを返す
}

bool wb::MouseMonitor::GetButtonDown(const MouseCode &mouseCode)
{
    if (mouseCode == MouseCode::Null || mouseCode >= MouseCode::Size)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"無効なマウスコードが指定されました。"}
        );
        wb::ErrorNotify("MOUSE_MONITOR", err);
        return false; // 無効なマウスコードの場合、falseを返す
    }

    if (mouseContext_->ButtonStates()[static_cast<size_t>(mouseCode)] == wb::INPUT_DOWN)
    {
        return true; // ボタンが押された状態
    }

    return false; // ボタンが押されていない状態
}

bool wb::MouseMonitor::GetButtonUp(const MouseCode &mouseCode)
{
    if (mouseCode == MouseCode::Null || mouseCode >= MouseCode::Size)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"無効なマウスコードが指定されました。"}
        );
        wb::ErrorNotify("MOUSE_MONITOR", err);
        return false; // 無効なマウスコードの場合、falseを返す
    }

    if (mouseContext_->ButtonStates()[static_cast<size_t>(mouseCode)] == wb::INPUT_UP)
    {
        return true; // ボタンが離された状態
    }

    return false; // ボタンが押されている状態
}

bool wb::MouseMonitor::GetButtonDoubleTap(const MouseCode &mouseCode, const double &threshold)
{
    if (mouseCode == MouseCode::Null || mouseCode >= MouseCode::Size)
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"無効なマウスコードが指定されました。"}
        );
        wb::ErrorNotify("MOUSE_MONITOR", err);
        return false; // 無効なマウスコードの場合、falseを返す
    }

    // ダブルタップは離された状態でのみチェック
    if (mouseContext_->ButtonStates()[static_cast<size_t>(mouseCode)] != wb::INPUT_UP) return false;

    // 前回のボタンが同じ場合、かつ前回の時間からの経過時間が閾値以下であればダブルタップ
    if 
    (
        mouseContext_->PreviousButton() == mouseCode &&
        std::chrono::duration<double>
        (
            std::chrono::high_resolution_clock::now() - mouseContext_->PreviousButtonTime()
        ).count() <= threshold
    ){
        return true; // ダブルタップが検出された
    }

    return false; // ダブルタップが検出されなかった
}

const int &wb::MouseMonitor::GetScrollDelta()
{
    return mouseContext_->ScrollDelta();
}

const UINT &wb::MouseMonitor::GetPositionX()
{
    return mouseContext_->PositionX();
}

const UINT &wb::MouseMonitor::GetPositionY()
{
    return mouseContext_->PositionY();
}