#include "wb_monitor/src/pch.h"
#include "wb_monitor/include/key_code_table.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

void wb::KeyCodeTable::SetUpTable()
{
    keyCodeMap_.clear();

    // キーコードを設定
    keyCodeMap_[{'A', false}] = wb::KeyCode::A;
    keyCodeMap_[{'B', false}] = wb::KeyCode::B;
    keyCodeMap_[{'C', false}] = wb::KeyCode::C;
    keyCodeMap_[{'D', false}] = wb::KeyCode::D;
    keyCodeMap_[{'E', false}] = wb::KeyCode::E;
    keyCodeMap_[{'F', false}] = wb::KeyCode::F;
    keyCodeMap_[{'G', false}] = wb::KeyCode::G;
    keyCodeMap_[{'H', false}] = wb::KeyCode::H;
    keyCodeMap_[{'I', false}] = wb::KeyCode::I;
    keyCodeMap_[{'J', false}] = wb::KeyCode::J;
    keyCodeMap_[{'K', false}] = wb::KeyCode::K;
    keyCodeMap_[{'L', false}] = wb::KeyCode::L;
    keyCodeMap_[{'M', false}] = wb::KeyCode::M;
    keyCodeMap_[{'N', false}] = wb::KeyCode::N;
    keyCodeMap_[{'O', false}] = wb::KeyCode::O;
    keyCodeMap_[{'P', false}] = wb::KeyCode::P;
    keyCodeMap_[{'Q', false}] = wb::KeyCode::Q;
    keyCodeMap_[{'R', false}] = wb::KeyCode::R;
    keyCodeMap_[{'S', false}] = wb::KeyCode::S;
    keyCodeMap_[{'T', false}] = wb::KeyCode::T;
    keyCodeMap_[{'U', false}] = wb::KeyCode::U;
    keyCodeMap_[{'V', false}] = wb::KeyCode::V;
    keyCodeMap_[{'W', false}] = wb::KeyCode::W;
    keyCodeMap_[{'X', false}] = wb::KeyCode::X;
    keyCodeMap_[{'Y', false}] = wb::KeyCode::Y;
    keyCodeMap_[{'Z', false}] = wb::KeyCode::Z;

    keyCodeMap_[{VK_RETURN, false}] = wb::KeyCode::Return;
    keyCodeMap_[{VK_ESCAPE, false}] = wb::KeyCode::Escape;
    keyCodeMap_[{VK_SPACE, false}] = wb::KeyCode::Space;
    keyCodeMap_[{VK_TAB, false}] = wb::KeyCode::Tab;
    keyCodeMap_[{VK_BACK, false}] = wb::KeyCode::BackSpace;

    keyCodeMap_[{VK_MENU, true}] = wb::KeyCode::RAlt;
    keyCodeMap_[{VK_MENU, false}] = wb::KeyCode::LAlt;
    keyCodeMap_[{VK_SHIFT, true}] = wb::KeyCode::RShift;
    keyCodeMap_[{VK_SHIFT, false}] = wb::KeyCode::LShift;
    keyCodeMap_[{VK_CONTROL, true}] = wb::KeyCode::RCtrl;
    keyCodeMap_[{VK_CONTROL, false}] = wb::KeyCode::LCtrl;

    keyCodeMap_[{VK_UP, false}] = wb::KeyCode::Up;
    keyCodeMap_[{VK_DOWN, false}] = wb::KeyCode::Down;
    keyCodeMap_[{VK_LEFT, false}] = wb::KeyCode::Left;
    keyCodeMap_[{VK_RIGHT, false}] = wb::KeyCode::Right;

    keyCodeMap_[{VK_INSERT, false}] = wb::KeyCode::Insert;
    keyCodeMap_[{VK_DELETE, false}] = wb::KeyCode::Del;
    keyCodeMap_[{VK_HOME, false}] = wb::KeyCode::Home;
    keyCodeMap_[{VK_END, false}] = wb::KeyCode::End;
    keyCodeMap_[{VK_PRIOR, false}] = wb::KeyCode::PageUp;
    keyCodeMap_[{VK_NEXT, false}] = wb::KeyCode::PageDown;
    keyCodeMap_[{VK_CAPITAL, false}] = wb::KeyCode::CapsLock;

    keyCodeMap_[{VK_F1, false}] = wb::KeyCode::F1;
    keyCodeMap_[{VK_F2, false}] = wb::KeyCode::F2;
    keyCodeMap_[{VK_F3, false}] = wb::KeyCode::F3;
    keyCodeMap_[{VK_F4, false}] = wb::KeyCode::F4;
    keyCodeMap_[{VK_F5, false}] = wb::KeyCode::F5;
    keyCodeMap_[{VK_F6, false}] = wb::KeyCode::F6;
    keyCodeMap_[{VK_F7, false}] = wb::KeyCode::F7;
    keyCodeMap_[{VK_F8, false}] = wb::KeyCode::F8;
    keyCodeMap_[{VK_F9, false}] = wb::KeyCode::F9;
    keyCodeMap_[{VK_F10, false}] = wb::KeyCode::F10;
    keyCodeMap_[{VK_F11, false}] = wb::KeyCode::F11;
    keyCodeMap_[{VK_F12, false}] = wb::KeyCode::F12;
    keyCodeMap_[{VK_F13, false}] = wb::KeyCode::F13;

    keyCodeMap_[{'0', false}] = wb::KeyCode::Alpha0;
    keyCodeMap_[{'1', false}] = wb::KeyCode::Alpha1;
    keyCodeMap_[{'2', false}] = wb::KeyCode::Alpha2;
    keyCodeMap_[{'3', false}] = wb::KeyCode::Alpha3;
    keyCodeMap_[{'4', false}] = wb::KeyCode::Alpha4;
    keyCodeMap_[{'5', false}] = wb::KeyCode::Alpha5;
    keyCodeMap_[{'6', false}] = wb::KeyCode::Alpha6;
    keyCodeMap_[{'7', false}] = wb::KeyCode::Alpha7;
    keyCodeMap_[{'8', false}] = wb::KeyCode::Alpha8;
    keyCodeMap_[{'9', false}] = wb::KeyCode::Alpha9;

    keyCodeMap_[{VK_NUMPAD0, false}] = wb::KeyCode::Numpad0;
    keyCodeMap_[{VK_NUMPAD1, false}] = wb::KeyCode::Numpad1;
    keyCodeMap_[{VK_NUMPAD2, false}] = wb::KeyCode::Numpad2;
    keyCodeMap_[{VK_NUMPAD3, false}] = wb::KeyCode::Numpad3;
    keyCodeMap_[{VK_NUMPAD4, false}] = wb::KeyCode::Numpad4;
    keyCodeMap_[{VK_NUMPAD5, false}] = wb::KeyCode::Numpad5;
    keyCodeMap_[{VK_NUMPAD6, false}] = wb::KeyCode::Numpad6;
    keyCodeMap_[{VK_NUMPAD7, false}] = wb::KeyCode::Numpad7;
    keyCodeMap_[{VK_NUMPAD8, false}] = wb::KeyCode::Numpad8;
    keyCodeMap_[{VK_NUMPAD9, false}] = wb::KeyCode::Numpad9;

    keyCodeMap_[{VK_OEM_1, false}] = wb::KeyCode::Oem1;
    keyCodeMap_[{VK_OEM_PLUS, false}] = wb::KeyCode::OemPlus;
    keyCodeMap_[{VK_OEM_COMMA, false}] = wb::KeyCode::OemComma;
    keyCodeMap_[{VK_OEM_MINUS, false}] = wb::KeyCode::OemMinus;
    keyCodeMap_[{VK_OEM_PERIOD, false}] = wb::KeyCode::OemPeriod;
    keyCodeMap_[{VK_OEM_2, false}] = wb::KeyCode::Oem2;
    keyCodeMap_[{VK_OEM_3, false}] = wb::KeyCode::Oem3;
    keyCodeMap_[{VK_OEM_4, false}] = wb::KeyCode::Oem4;
    keyCodeMap_[{VK_OEM_5, false}] = wb::KeyCode::Oem5;
    keyCodeMap_[{VK_OEM_6, false}] = wb::KeyCode::Oem6;
    keyCodeMap_[{VK_OEM_7, false}] = wb::KeyCode::Oem7;
    keyCodeMap_[{VK_OEM_102, false}] = wb::KeyCode::Oem102;
}

bool wb::KeyCodeTable::IsSetUp() const
{
    if (keyCodeMap_.empty())
    {
        std::string err = wb::ConsoleLogErr
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"キーコードテーブルが設定されていません。"}
        );
        wb::ErrorNotify("KEY_CODE_TABLE", err);
        return false;
    }

    return true;
}

wb::KeyCode wb::KeyCodeTable::GetKeyCode(WPARAM wParam, LPARAM lParam) const
{
    // 右側のキーかどうかを判定
    bool isExtended = (lParam & (1 << 24)) != 0;

    // キーが対応しているか確認
    std::pair<WPARAM, bool> keyPair = { wParam, isExtended };
    bool isKeyFound = keyCodeMap_.find(keyPair) != keyCodeMap_.end();
    if (!isKeyFound) return KeyCode::Null;

    // 対応するキーコードを取得
    return keyCodeMap_.at(keyPair);
}