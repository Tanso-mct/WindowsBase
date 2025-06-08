#include "wb_monitor/src/pch.h"
#include "wb_monitor/include/mouse_input_type_table.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

void wb::MouseInputTypeTable::SetUpTable()
{
    inputTypeMap_.clear();

    // マウスの入力タイプを設定
    inputTypeMap_[WM_LBUTTONDOWN] = INPUT_DOWN;
    inputTypeMap_[WM_LBUTTONUP] = INPUT_UP;

    inputTypeMap_[WM_RBUTTONDOWN] = INPUT_DOWN;
    inputTypeMap_[WM_RBUTTONUP] = INPUT_UP;

    inputTypeMap_[WM_MBUTTONDOWN] = INPUT_DOWN;
    inputTypeMap_[WM_MBUTTONUP] = INPUT_UP;

    inputTypeMap_[WM_XBUTTONDOWN] = INPUT_DOWN;
    inputTypeMap_[WM_XBUTTONUP] = INPUT_UP;
}

bool wb::MouseInputTypeTable::IsSetUp() const
{
    if (inputTypeMap_.empty())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"マウス入力タイプテーブルが設定されていません。"}
        );
        wb::ErrorNotify("MOUSE_INPUT_TYPE_TABLE", err);
        return false;
    }

    return true;
}

UINT wb::MouseInputTypeTable::GetInputType(UINT msg) const
{
    bool isInputTypeFound = inputTypeMap_.find(msg) != inputTypeMap_.end();
    if (!isInputTypeFound) return INPUT_NONE; // 対応する入力タイプが見つからない場合はINPUT_NONEを返す

    // 対応する入力タイプを取得
    return inputTypeMap_.at(msg);
}