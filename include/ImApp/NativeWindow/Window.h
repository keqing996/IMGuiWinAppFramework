#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <functional>
#include "WindowStyle.h"
#include "ImApp/Utility/NonCopyable.h"

namespace NativeWindow
{
    class NativeWindowUtility;

    class Window: Utility::NonCopyable
    {
        friend NativeWindowUtility;
    public:
        using WindowHandle = void*;
        using IconHandle = void*;
        using CursorHandle = void*;
        using DeviceContextHandle = void*;

    public:
        Window(int width, int height, const std::string& title, int style = static_cast<int>(WindowStyle::Default));
        virtual ~Window();

    public:
        void EventLoop();
        void SetWindowEventProcessFunction(const std::function<bool(void*, uint32_t, void*, void*)>& f);
        void ClearWindowEventProcessFunction();

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

    protected:
        virtual bool WindowEventPreProcess(uint32_t message, void* wpara, void* lpara);
        virtual void OnWindowClose();
        virtual void OnWindowResize(int width, int height);
        virtual void OnWindowGetFocus();
        virtual void OnWindowLostFocus();
        virtual void OnMouseEnterWindow();
        virtual void OnMouseLeaveWindow();

    private:
        void WindowEventProcess(uint32_t message, void* wpara, void* lpara);
        void WindowEventProcessInternal(uint32_t message, void* wpara, void* lpara);
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