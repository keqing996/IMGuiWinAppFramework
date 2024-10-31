#pragma once

namespace NativeWindow
{
    struct WindowState
    {
        void* hWindow = nullptr;
        void* hIcon = nullptr;
        void* hCursor = nullptr;

        bool cursorVisible = true;
        bool cursorLimitedInWindow = false;
        bool cursorInsideWindow = false;

        int lastWidth = 0;
        int lastHeight = 0;
    };
}