#pragma once

#include <cstdint>
#include <string>
#include <queue>
#include <optional>
#include <functional>
#include "WindowEvent.h"
#include "WindowStyle.h"

namespace NativeWindow
{
    class NativeWindowUtility;

    class Window
    {
        friend NativeWindowUtility;
    public:
        using WindowHandle = void*;
        using IconHandle = void*;
        using CursorHandle = void*;
        using DeviceContextHandle = void*;

    public:
        Window(int width, int height, const std::string& title, int style = static_cast<int>(WindowStyle::Default));
        ~Window();

    public:
        //
        void EventLoop();
        void SetWindowEventProcessFunction(const std::function<bool(void*, uint32_t, void*, void*)>& f);
        void ClearWindowEventProcessFunction();
        bool HasEvent() const;
        WindowEvent PopEvent();
        std::vector<WindowEvent> PopAllEvent();

        std::pair<int, int> GetSize();
        void SetSize(int width, int height);

        std::pair<int, int> GetPosition();
        void SetPosition(int x, int y);

        void* GetSystemHandle() const;

        void SetIcon(unsigned int width, unsigned int height, const std::byte* pixels);
        void SetIcon(int iconResId);

        void SetTitle(const std::string& title);

        void SetWindowVisible(bool show);

        bool GetCursorVisible() const;
        void SetCursorVisible(bool show);

        bool GetCursorCapture() const;
        void SetCursorCapture(bool capture);

        bool GetKeyRepeated() const;
        void SetKeyRepeated(bool repeated);

    private:
        void WindowEventProcess(uint32_t message, void* wpara, void* lpara);
        void WindowEventProcessInternal(uint32_t message, void* wpara, void* lpara);
        void PushEvent(const WindowEvent& event);
        void CaptureCursorInternal(bool doCapture);

    private:
        // Window handle
        WindowHandle _hWindow;
        DeviceContextHandle _hDeviceHandle;

        // State
        std::pair<int, int> _windowSize;
        bool _enableKeyRepeat;
        bool _cursorVisible;
        bool _cursorCapture;
        bool _mouseInsideWindow;

        // Resource
        IconHandle _hIcon;
        CursorHandle _hCursor;

        // Event
        std::queue<WindowEvent> _eventQueue;

        // Additional handler
        std::function<bool(void*, uint32_t, void*, void*)> _winEventProcess;

    private:
        static void RegisterWindowClass();
        static void UnRegisterWindowClass();

    private:
        inline static int _sGlobalWindowsCount = 0;
        inline static const wchar_t* _sWindowRegisterName = L"InfraWindow";
    };
}