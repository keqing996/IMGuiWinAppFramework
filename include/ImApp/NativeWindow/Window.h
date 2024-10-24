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
        bool Create(int width, int height, const std::string& title);
        bool Create(int width, int height, const std::string& title, WindowStyle style);
        void Destroy();
        bool IsWindowValid() const;

        /// Process windows messages.
        /// @param windowDestroyed True is window destroyed after message process.
        void EventLoop(bool* windowDestroyed);

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

    protected:
        virtual void OnWindowInitialized();

        /// Called when windows messages received.
        /// @param message Windows message.
        /// @param wpara WPARAM.
        /// @param lpara LPARAM.
        /// @param result If block original message process, return value of message.
        /// @return Should block original message process.
        virtual bool WindowEventPreProcess(uint32_t message, void* wpara, void* lpara, int* result);
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

    private:
        friend NativeWindowUtility;

    private:
        std::unique_ptr<WindowState> _pWindowState = nullptr;

    private:
        static void RegisterWindowClass();
        static void UnRegisterWindowClass();
    };
}