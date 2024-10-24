#pragma once

namespace NativeWindow
{
    struct WindowState
    {
        void* hWindow = nullptr;
        void* hIcon = nullptr;
        void* hCursor = nullptr;
        int width = 0;
        int height = 0;
        bool cursorVisible = true;
        bool cursorCapture = false;
        bool mouseInsideWindow = false;
    };
}