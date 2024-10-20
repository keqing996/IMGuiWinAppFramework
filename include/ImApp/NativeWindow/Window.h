#pragma once

#include "WindowEvent.h"
#include <cstdint>
#include <string>
#include <queue>
#include <optional>
#include <functional>

namespace NativeWindow
{
    enum class WindowStyle: int
    {
        None = 0,
        HaveTitleBar = 1 << 0,
        HaveResize = 1 << 1,
        HaveClose = 1 << 2,
        Default = HaveTitleBar | HaveResize | HaveClose
    };

    const int DefaultWindowStyle = static_cast<int>(WindowStyle::Default);

    class Window
    {
    public:
        using WindowHandle = void*;
        using IconHandle = void*;
        using CursorHandle = void*;
        using DeviceContextHandle = void*;

    public:
        Window(int width, int height, const std::string& title, int style = (int)WindowStyle::Default);
        ~Window();

    public:
        auto EventLoop() -> void;
        auto WindowEventProcess(uint32_t message, void* wpara, void* lpara) -> void;
        auto SetWindowEventProcessFunction(const std::function<bool(void*, uint32_t, void*, void*)>& f) -> void;
        auto ClearWindowEventProcessFunction() -> void;
        auto HasEvent() -> bool;
        auto PopEvent() -> WindowEvent;
        auto PopAllEvent() -> std::vector<WindowEvent>;

        auto GetSize() -> std::pair<int, int>;
        auto SetSize(int width, int height) -> void;

        auto GetPosition() -> std::pair<int, int>;
        auto SetPosition(int x, int y) -> void;

        auto GetSystemHandle() const -> void*;

        auto SetIcon(unsigned int width, unsigned int height, const std::byte* pixels) -> void;
        auto SetIcon(int iconResId) -> void;

        auto SetTitle(const std::string& title) -> void;

        auto SetWindowVisible(bool show) -> void;

        auto GetCursorVisible() -> bool;
        auto SetCursorVisible(bool show) -> void;

        auto GetCursorCapture() -> bool;
        auto SetCursorCapture(bool capture) -> void;

        auto GetKeyRepeated() -> bool;
        auto SetKeyRepeated(bool repeated) -> void;

    private:
        auto WindowEventProcessInternal(uint32_t message, void* wpara, void* lpara) -> void;
        auto PushEvent(const WindowEvent& event) -> void;
        auto CaptureCursorInternal(bool doCapture) -> void;

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