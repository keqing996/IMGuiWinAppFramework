#pragma once

#include "NativeWindow/Window.h"
#include "NativeWindow/WindowsInclude.h"

namespace NativeWindow
{
    class NativeWindowUtility
    {
    public:
        NativeWindowUtility() = delete;

    public:
        static bool FixProcessDpi();

        static LRESULT CALLBACK DefaultWndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

        static std::pair<int, int> CalculateAdjustWindowSize(int width, int height, DWORD dwStyle);

    private:
        static bool FixProcessDpiBySetProcessDpiAwareness();

        static bool FixProcessDpiBySetProcessDPIAware();
    };
}