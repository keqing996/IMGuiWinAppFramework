#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <functional>
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
        bool Initialize(int width, int height, const std::string& title);
        bool Initialize(int width, int height, const std::string& title, WindowStyle style);
        void Clear();

        bool EventLoop();
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
        virtual void OnWindowInitialized();
        virtual bool WindowEventPreProcess(uint32_t message, void* wpara, void* lpara);
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