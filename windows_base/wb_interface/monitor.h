#pragma once

#include "wb_interface/data_config.h"

#include <vector>
#include <chrono>
#include <ctime>
#include <Windows.h>
#include <memory>
#include <unordered_map>

namespace wb
{
    /*******************************************************************************************************************
     * 入力状態のビット演算で使用するための定数
    /******************************************************************************************************************/

    constexpr UINT INPUT_NONE = 0;
    constexpr UINT INPUT_DOWN = 1;
    constexpr UINT INPUT_PRESSED = 2;
    constexpr UINT INPUT_UP = 3;

    /*******************************************************************************************************************
     * ウィンドウメッセージからそれの入力種類を取得するためのテーブル
    /******************************************************************************************************************/

    class IInputTypeTable
    {
    public:
        virtual ~IInputTypeTable() = default;

        virtual void SetUpTable() = 0;
        virtual bool IsSetUp() const = 0;

        virtual UINT GetInputType(UINT msg) const = 0;
    };

    /*******************************************************************************************************************
     * Keyboardのキーを識別するコードと、WPARAM、LPARAMからKeyCodeを取得するためのTableクラス
     * これを使用して、キーの状態を管理する、キーの状態の取得などを行う
    /******************************************************************************************************************/

    enum class KeyCode : size_t
    {
        Null,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Return, Escape, Space, Tab, BackSpace,
        RAlt, LAlt, LShift, RShift, LCtrl, RCtrl,
        Up, Down, Left, Right,
        Insert, Del, Home, End, PageUp, PageDown, CapsLock,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13,
        Alpha0, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9, NumpadEnter,
        Oem1, OemPlus, OemComma, OemMinus, OemPeriod, Oem2, Oem3, Oem4, Oem5, Oem6, Oem7, Oem102,
        Size
    };

    class IKeyCodeTable
    {
    public:
        virtual ~IKeyCodeTable() = default;

        virtual void SetUpTable() = 0;
        virtual bool IsSetUp() const = 0;

        virtual KeyCode GetKeyCode(WPARAM wParam, LPARAM lParam) const = 0;
    };

    /*******************************************************************************************************************
     * Keyboardの状態を管理するためのContext
    /******************************************************************************************************************/

    class IKeyboardContext
    {
    public:
        virtual ~IKeyboardContext() = default;

        virtual std::vector<UINT> &KeyStates() = 0;
        virtual KeyCode &PreviousKey() = 0;
        virtual std::chrono::high_resolution_clock::time_point &PreviousKeyTime() = 0;
    };

    /*******************************************************************************************************************
     * Mouseのボタンを識別するコードと、WPARAM、LPARAMからMouseCodeを取得するためのTableクラス
     * これを使用して、Mouseのボタンの状態を管理する、ボタンの状態の取得などを行う
    /******************************************************************************************************************/

    enum class MouseCode : size_t
    {
        Null, Left, Right, Middle, X1, X2, Size
    };

    class IMouseCodeTable
    {
    public:
        virtual ~IMouseCodeTable() = default;

        virtual void SetUpTable() = 0;
        virtual bool IsSetUp() const = 0;

        virtual MouseCode GetMouseCode(UINT msg, WPARAM wPram) const = 0;
    };

    /*******************************************************************************************************************
     * Mouseの状態を管理するためのContext
    /******************************************************************************************************************/

    class IMouseContext
    {
    public:
        virtual ~IMouseContext() = default;

        virtual std::vector<UINT> &ButtonStates() = 0;
        virtual MouseCode &PreviousButton() = 0;
        virtual std::chrono::high_resolution_clock::time_point &PreviousButtonTime() = 0;

        virtual int &ScrollDelta() = 0;
        virtual UINT &PositionX() = 0;
        virtual UINT &PositionY() = 0;
    };

    /*******************************************************************************************************************
     * 各種Monitorの基底インターフェースの
     * Monitorは必ずIMonitorを継承する必要がある
    /******************************************************************************************************************/

    class IMonitor
    {
    public:
        virtual ~IMonitor() = default;

        virtual MonitorType GetType() const = 0;
        virtual void UpdateState() = 0;
    };

    /*******************************************************************************************************************
     * 各種Monitorのインターフェース
    /******************************************************************************************************************/

    class IKeyboardMonitor : public IMonitor
    {
    public:
        virtual ~IKeyboardMonitor() = default;

        virtual void SetUpContext(std::unique_ptr<IKeyboardContext> keyContext) = 0;
        virtual void SetUpCodeTable(std::unique_ptr<IKeyCodeTable> keyCodeTable) = 0;
        virtual bool IsSetUp() const = 0;

        virtual void EditStateByDown(WPARAM wParam, LPARAM lParam) = 0;
        virtual void EditStateByUp(WPARAM wParam, LPARAM lParam) = 0;

        virtual bool GetKey(const KeyCode &keyCode) = 0;
        virtual bool GetKeyDown(const KeyCode &keyCode) = 0;
        virtual bool GetKeyUp(const KeyCode &keyCode) = 0;
        virtual bool GetKeyDoubleTap(const KeyCode &keyCode, const double &threshold) = 0;
    };

    class IMouseMonitor : public IMonitor
    {
    public:
        virtual ~IMouseMonitor() = default;

        virtual void SetUpContext(std::unique_ptr<IMouseContext> context) = 0;
        virtual void SetUpCodeTable(std::unique_ptr<IMouseCodeTable> mouseCodeTable) = 0;
        virtual void SetUpInputTypeTable(std::unique_ptr<IInputTypeTable> inputTypeTable) = 0;
        virtual bool IsSetUp() const = 0;

        virtual void EditState(UINT msg, WPARAM wParam, LPARAM lParam) = 0;

        virtual bool GetButton(const MouseCode &mouseCode) = 0;
        virtual bool GetButtonDown(const MouseCode &mouseCode) = 0;
        virtual bool GetButtonUp(const MouseCode &mouseCode) = 0;
        virtual bool GetButtonDoubleTap(const MouseCode &mouseCode, const double &threshold) = 0;

        virtual const int &GetScrollDelta() = 0;
        virtual const UINT &GetPositionX() = 0;
        virtual const UINT &GetPositionY() = 0;
    };

    /*******************************************************************************************************************
     * Monitorのファクトリーインターフェース
     * Collectionクラスなどで使用する
    /******************************************************************************************************************/

    class IMonitorFactory
    {
    public:
        virtual ~IMonitorFactory() = default;
        virtual std::unique_ptr<IMonitor> Create() = 0;
    };
    
} // namespace wb