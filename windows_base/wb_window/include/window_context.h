#pragma once

#include "wb_window/include/dll_config.h"

#include "wb_interface/window.h"

namespace wb
{
    class WB_WINDOW_API WindowContext : public IWindowContext
    {
    private:
        HWND handle_ = nullptr;
        HINSTANCE instance_ = nullptr;
        HWND parentHandle_ = nullptr;

        DWORD style_ = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
        std::wstring name_ = L"";
        UINT posX_ = CW_USEDEFAULT;
        UINT posY_ = CW_USEDEFAULT;
        UINT width_ = DEFAULT_WINDOW_WIDTH;
        UINT height_ = DEFAULT_WINDOW_HEIGHT;
        UINT clientWidth_ = 0;
        UINT clientHeight_ = 0;

        bool isCreated_ = false;
        bool isFocus_ = false;
        bool isMaximized_ = false;
        bool isMinimized_ = false;
        bool isFullScreen_ = false;

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
        /**************************************************************************************************************/

        WindowContext() = default;
        virtual ~WindowContext() = default;

        /***************************************************************************************************************
         * IWindowContextインターフェースの実装
        /**************************************************************************************************************/

        HWND &Handle() override { return handle_; }
        HINSTANCE &Instance() override { return instance_; }
        HWND &ParentHandle() override { return parentHandle_; }

        DWORD &Style() override { return style_; }
        std::wstring &Name() override { return name_; }
        UINT &PosX() override { return posX_; }
        UINT &PosY() override { return posY_; }
        UINT &Width() override { return width_; }
        UINT &Height() override { return height_; }
        UINT &ClientWidth() override { return clientWidth_; }
        UINT &ClientHeight() override { return clientHeight_; }

        bool &IsCreated() override { return isCreated_; }
        bool &IsFocus() override { return isFocus_; }
        bool &IsMaximized() override { return isMaximized_; }
        bool &IsMinimized() override { return isMinimized_; }
        bool &IsFullScreen() override { return isFullScreen_; }
    };

} // namespace wb