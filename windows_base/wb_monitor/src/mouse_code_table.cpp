#include "wb_monitor/src/pch.h"
#include "wb_monitor/include/mouse_code_table.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

void wb::MouseCodeTable::SetUpTable()
{
    mouseCodeMap_.clear();

    // マウスコードを設定
    mouseCodeMap_[{WM_LBUTTONDOWN, wb::NOT_XBUTTON}] = wb::MouseCode::Left;
    mouseCodeMap_[{WM_LBUTTONUP, wb::NOT_XBUTTON}] = wb::MouseCode::Left;

    mouseCodeMap_[{WM_RBUTTONDOWN, wb::NOT_XBUTTON}] = wb::MouseCode::Right;
    mouseCodeMap_[{WM_RBUTTONUP, wb::NOT_XBUTTON}] = wb::MouseCode::Right;

    mouseCodeMap_[{WM_MBUTTONDOWN, wb::NOT_XBUTTON}] = wb::MouseCode::Middle;
    mouseCodeMap_[{WM_MBUTTONUP, wb::NOT_XBUTTON}] = wb::MouseCode::Middle;

    mouseCodeMap_[{WM_XBUTTONDOWN, XBUTTON1}] = wb::MouseCode::X1;
    mouseCodeMap_[{WM_XBUTTONUP, XBUTTON1}] = wb::MouseCode::X1;

    mouseCodeMap_[{WM_XBUTTONDOWN, XBUTTON2}] = wb::MouseCode::X2;
    mouseCodeMap_[{WM_XBUTTONUP, XBUTTON2}] = wb::MouseCode::X2;
}

bool wb::MouseCodeTable::IsSetUp() const
{
    if (mouseCodeMap_.empty())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"マウスコードテーブルが設定されていません。"}
        );
        wb::ErrorNotify("MOUSE_CODE_TABLE", err);
        return false;
    }

    return true;
}

wb::MouseCode wb::MouseCodeTable::GetMouseCode(UINT msg, WPARAM wParam) const
{
    if 
    (
        GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ||
        GET_XBUTTON_WPARAM(wParam) == XBUTTON2
    ){
        std::pair<UINT, UINT> buttonPair = {msg, GET_XBUTTON_WPARAM(wParam)};
        bool isButtonFound = mouseCodeMap_.find(buttonPair) != mouseCodeMap_.end();
        if (!isButtonFound) return MouseCode::Null; // 対応するボタンが見つからない場合はNullを返す

        // 対応するマウスコードを取得
        return mouseCodeMap_.at(buttonPair);
    }
    else
    {
        std::pair<UINT, UINT> buttonPair = {msg, wb::NOT_XBUTTON};
        bool isButtonFound = mouseCodeMap_.find(buttonPair) != mouseCodeMap_.end();
        if (!isButtonFound) return MouseCode::Null; // 対応するボタンが見つからない場合はNullを返す

        // 対応するマウスコードを取得
        return mouseCodeMap_.at(buttonPair);
    }
}