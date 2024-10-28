#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <memory>

#include "WindowStyle.h"
#include "WindowState.h"
#include "ImApp/Utility/NonCopyable.h"

namespace NativeWindow
{
    class NativeWindowUtility;

    class Window: Utility::NonCopyable
    {
    public:
        Window();
        virtual ~Window();

    public:
        /// Create window instance.
        /// @param width Window width.
        /// @param height Window height.
        /// @param title Window title in UTF8.
        /// @param style Window style.
        /// @return Create success.
        virtual
        bool Create(int width, int height, const std::string& title, WindowStyle style);

        /// Destroy window instance.
        void Destroy();

        /// Check is window instance created.
        bool IsWindowValid() const;

        /// Process windows messages.
        /// @param windowDestroyed True is window destroyed after message process.
        void EventLoop(bool* windowDestroyed);

        /// Get native window's size.
        std::pair<int, int> GetSize();

        /// Set native window's size.
        void SetSize(int width, int height);

        /// Get native window's position in screen.
        std::pair<int, int> GetPosition();

        /// Set native window's position in screen.
        void SetPosition(int x, int y);

        /// Get native window handle, which is HWND in this case.
        void* GetSystemHandle() const;

        /// Set icon of window through bytes in memory.
        /// @param width Icon width.
        /// @param height Icon height.
        /// @param pixels Icon bytes data in RGBA.
        void SetIcon(unsigned int width, unsigned int height, const std::byte* pixels);

        /// Set icon of window through windows resource id.
        void SetIcon(int iconResId);

        /// Set window title in UTF8.
        void SetTitle(const std::string& title);

        void SetWindowVisible(bool show);

        bool IsCursorVisible() const;
        virtual void SetCursorVisible(bool show);

        bool GetCursorCapture() const;
        void SetCursorCapture(bool capture);

        bool IsMouseInsideWindow() const;

    protected:
        virtual void OnWindowCreated();

        /// Called when windows messages received.
        /// @param message Windows message.
        /// @param wpara WPARAM.
        /// @param lpara LPARAM.
        /// @param result If block original message process, return value of message.
        /// @return Should block original message process.
        virtual bool NativeWindowEventPreProcess(uint32_t message, void* wpara, void* lpara, int* result);
        virtual void OnWindowClose();
        virtual void OnWindowPreDestroy();
        virtual void OnWindowPostDestroy();
        virtual void OnWindowResize(int width, int height);
        virtual void OnWindowGetFocus();
        virtual void OnWindowLostFocus();
        virtual void OnMouseEnterWindow();
        virtual void OnMouseLeaveWindow();

    private:
        int WindowEventProcess(uint32_t message, void* wpara, void* lpara);
        void WindowEventProcessInternal(uint32_t message, void* wpara, void* lpara);
        void CaptureCursorInternal(bool doCapture);
        bool CalculateMouseInsideWindow() const;

    private:
        friend NativeWindowUtility;

    private:
        std::unique_ptr<WindowState> _pWindowState = nullptr;

    private:
        static void RegisterWindowClass();
        static void UnRegisterWindowClass();
    };
}