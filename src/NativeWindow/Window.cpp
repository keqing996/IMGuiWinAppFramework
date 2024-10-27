
#include "ImApp/Utility/WindowsInclude.h"
#include "ImApp/Utility/String.h"
#include "ImApp/NativeWindow/Window.h"

#include <vector>

#include "NativeWindowUtility.h"

// Serval paths of window destroyed.
// 1. Window receive WM_CLOSE, pressed close button for example.
//    OnWindowClose -> DestroyWindow -> WM_DESTROY -> OnWindowDestroy
// 2. Call Window::Clear
//    Window::Clear -> DestroyWindow -> WM_DESTROY -> OnWindowDestroy

namespace NativeWindow
{
    static int gGlobalWindowsCount = 0;
    static const wchar_t* gWindowRegisterName = L"InfraWindow";

    Window::Window()
    {
        NativeWindowUtility::FixProcessDpi();
    }

    Window::~Window()
    {
        Destroy();
    }

    bool Window::Create(int width, int height, const std::string& title, WindowStyle style)
    {
        // Register window
        if (gGlobalWindowsCount == 0)
            RegisterWindowClass();

        // Get create style
        DWORD win32Style = WS_VISIBLE;
        if (style.type == WindowStyle::Type::Popup)
            win32Style |= WS_POPUP;
        else
        {
            if (style.config.overlapped.haveTitleBar)
                win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
            if (style.config.overlapped.haveResize)
                win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
            if (style.config.overlapped.haveClose)
                win32Style |= WS_SYSMENU;
        }

        // Calculate create position: left & top
        const HDC screenDC = ::GetDC(nullptr);
        const int left = (::GetDeviceCaps(screenDC, HORZRES) - width) / 2;
        const int top = (::GetDeviceCaps(screenDC, VERTRES) - height) / 2;
        ::ReleaseDC(nullptr, screenDC);

        // Adjust create size
        auto [adjustWidth, adjustHeight] = NativeWindowUtility::CalculateAdjustWindowSize(width, height, win32Style);

        // Create window
        const auto titleInWideStr = Utility::StringToWideString(title);
        const wchar_t* titleWide = titleInWideStr.c_str();
        auto hWindow = ::CreateWindowW(
                gWindowRegisterName,
                titleWide,
                win32Style,
                left,
                top,
                adjustWidth,
                adjustHeight,
                nullptr,
                nullptr,
                ::GetModuleHandleW(nullptr),
                this);

        if (!hWindow)
            return false;

        // Global counting
        gGlobalWindowsCount++;

        // Create window state
        _pWindowState = std::make_unique<WindowState>();
        _pWindowState->hWindow = hWindow;
        _pWindowState->width = width;
        _pWindowState->height = height;

        // Cursor
        _pWindowState->hCursor = ::LoadCursor(nullptr, IDC_ARROW);
        _pWindowState->cursorVisible = true;
        SetWindowVisible(true);
        _pWindowState->cursorCapture = false;
        SetCursorCapture(false);
        _pWindowState->mouseInsideWindow = CalculateMouseInsideWindow();

        // Set size again after window creation to avoid some bug.
        SetSize(width, height);

        OnWindowCreated();

        return true;
    }

    void Window::Destroy()
    {
        if (_pWindowState == nullptr)
            return;

        // Destroy window
        ::DestroyWindow(static_cast<HWND>(_pWindowState->hWindow));
    }

    bool Window::IsWindowValid() const
    {
        return _pWindowState != nullptr;
    }

    void Window::RegisterWindowClass()
    {
        WNDCLASSW windowClass {};
        windowClass.style         = 0;
        windowClass.lpfnWndProc   = &NativeWindowUtility::DefaultWndProc;
        windowClass.cbClsExtra    = 0;
        windowClass.cbWndExtra    = 0;
        windowClass.hInstance     = ::GetModuleHandleW(nullptr);
        windowClass.hIcon         = nullptr;
        windowClass.hCursor       = nullptr;
        windowClass.hbrBackground = nullptr;
        windowClass.lpszMenuName  = nullptr;
        windowClass.lpszClassName = gWindowRegisterName;
        ::RegisterClassW(&windowClass);
    }

    void Window::UnRegisterWindowClass()
    {
        ::UnregisterClassW(gWindowRegisterName, GetModuleHandleW(nullptr));
    }

    void Window::SetSize(int width, int height)
    {
        if (_pWindowState == nullptr)
            return;

        const HWND hWnd = static_cast<HWND>(_pWindowState->hWindow);
        const DWORD dwStyle = static_cast<DWORD>(::GetWindowLongPtrW(hWnd, GWL_STYLE));
        auto [adjustWidth, adjustHeight] = NativeWindowUtility::CalculateAdjustWindowSize(width, height, dwStyle);
        ::SetWindowPos(hWnd, nullptr, 0, 0, adjustWidth, adjustHeight, SWP_NOMOVE | SWP_NOZORDER);
    }

    void* Window::GetSystemHandle() const
    {
        if (_pWindowState == nullptr)
            return nullptr;

        return _pWindowState->hWindow;
    }

    void Window::SetIcon(unsigned int width, unsigned int height, const std::byte* pixels)
    {
        if (_pWindowState == nullptr)
            return;

        if (_pWindowState->hIcon != nullptr)
            ::DestroyIcon(static_cast<HICON>(_pWindowState->hIcon));

        _pWindowState->hIcon = nullptr;

        // Change RGBA to BGRA
        std::vector<std::byte> iconPixels(width * height * 4);
        for (std::size_t i = 0; i < iconPixels.size() / 4; ++i)
        {
            iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
            iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
            iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
            iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
        }

        _pWindowState->hIcon = ::CreateIcon(
                GetModuleHandleW(nullptr),
                static_cast<int>(width),
                static_cast<int>(height),
                1,
                32,
                nullptr,
                reinterpret_cast<unsigned char*>(iconPixels.data()));

        if (_pWindowState->hIcon != nullptr)
        {
            ::SendMessageW(
                    static_cast<HWND>(_pWindowState->hWindow),
                    WM_SETICON,
                    ICON_BIG,
                    reinterpret_cast<LPARAM>(_pWindowState->hIcon));

            ::SendMessageW(
                    static_cast<HWND>(_pWindowState->hWindow),
                    WM_SETICON,
                    ICON_SMALL,
                    reinterpret_cast<LPARAM>(_pWindowState->hIcon));
        }
    }

    void Window::SetIcon(int iconResId)
    {
        if (_pWindowState == nullptr)
            return;

        if (_pWindowState->hIcon != nullptr)
            ::DestroyIcon(static_cast<HICON>(_pWindowState->hIcon));

        _pWindowState->hIcon = nullptr;

        _pWindowState->hIcon = ::LoadIconW(GetModuleHandleW(nullptr), MAKEINTRESOURCE(iconResId));
        if (_pWindowState->hIcon != nullptr)
        {
            ::SendMessageW(
                    static_cast<HWND>(_pWindowState->hWindow),
                    WM_SETICON,
                    ICON_BIG,
                    reinterpret_cast<LPARAM>(_pWindowState->hIcon));

            ::SendMessageW(
                    static_cast<HWND>(_pWindowState->hWindow),
                    WM_SETICON,
                    ICON_SMALL,
                    reinterpret_cast<LPARAM>(_pWindowState->hIcon));
        }
    }

    void Window::SetWindowVisible(bool show)
    {
        if (_pWindowState == nullptr)
            return;

        ::ShowWindow(static_cast<HWND>(_pWindowState->hWindow), show ? SW_SHOW : SW_HIDE);
    }

    void Window::SetCursorVisible(bool show)
    {
        if (_pWindowState == nullptr)
            return;

        _pWindowState->cursorVisible = show;
        ::SetCursor(show ? static_cast<HCURSOR>(_pWindowState->hCursor) : nullptr);
    }

    void Window::SetCursorCapture(bool capture)
    {
        if (_pWindowState == nullptr)
            return;

        _pWindowState->cursorCapture = capture;
        CaptureCursorInternal(_pWindowState->cursorCapture);
    }

    bool Window::IsMouseInsideWindow() const
    {
        if (_pWindowState == nullptr)
            return false;

        return _pWindowState->mouseInsideWindow;
    }

    bool Window::GetCursorVisible() const
    {
        if (_pWindowState == nullptr)
            return _pWindowState->cursorVisible;

        return false;
    }

    bool Window::GetCursorCapture() const
    {
        if (_pWindowState == nullptr)
            return _pWindowState->cursorCapture;

        return false;
    }

    void Window::OnWindowCreated()
    {
    }

    bool Window::NativeWindowEventPreProcess(uint32_t message, void* wpara, void* lpara, int* result)
    {
        *result = 0;
        return false;
    }

    void Window::OnWindowClose()
    {
        if (_pWindowState == nullptr)
            return;

        ::DestroyWindow(static_cast<HWND>(_pWindowState->hWindow));
    }

    void Window::OnWindowPreDestroy()
    {
        if (_pWindowState == nullptr)
            return;

        SetCursorVisible(true);
        ::ReleaseCapture();

        // Icon
        if (_pWindowState->hIcon != nullptr)
            ::DestroyIcon(static_cast<HICON>(_pWindowState->hIcon));
    }

    void Window::OnWindowPostDestroy()
    {
        _pWindowState.reset();

        // Global counting
        gGlobalWindowsCount--;

        if (gGlobalWindowsCount == 0)
            UnRegisterWindowClass();
    }

    void Window::OnWindowResize(int width, int height)
    {
    }

    void Window::OnWindowGetFocus()
    {
    }

    void Window::OnWindowLostFocus()
    {
    }

    void Window::OnMouseEnterWindow()
    {
    }

    void Window::OnMouseLeaveWindow()
    {
    }

    void Window::SetTitle(const std::string& title)
    {
        if (_pWindowState == nullptr)
            return;

        const auto titleInWideStr = Utility::StringToWideString(title);
        const wchar_t* titleWide = titleInWideStr.c_str();
        ::SetWindowTextW(static_cast<HWND>(_pWindowState->hWindow), titleWide);
    }

    std::pair<int, int> Window::GetSize()
    {
        if (_pWindowState == nullptr)
            return { 0, 0 };

        RECT rect;
        ::GetClientRect(static_cast<HWND>(_pWindowState->hWindow), &rect);
        return { static_cast<int>(rect.right - rect.left), static_cast<int>(rect.bottom - rect.top) };
    }

    std::pair<int, int> Window::GetPosition()
    {
        if (_pWindowState == nullptr)
            return { 0, 0 };

        RECT rect;
        ::GetWindowRect(static_cast<HWND>(_pWindowState->hWindow), &rect);

        return { static_cast<int>(rect.left), static_cast<int>(rect.top) };
    }

    void Window::SetPosition(int x, int y)
    {
        if (_pWindowState == nullptr)
            return;

        ::SetWindowPos(
                static_cast<HWND>(_pWindowState->hWindow),
                nullptr,
                x,
                y,
                0,
                0,
                SWP_NOSIZE | SWP_NOZORDER);

        // Adjust cursor position
        if(_pWindowState->cursorCapture)
            SetCursorCapture(true);
    }

    void Window::EventLoop(bool* windowDestroyed)
    {
        // Fetch new event
        MSG message;
        while (::PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&message);
            ::DispatchMessageW(&message);
        }

        *windowDestroyed = !IsWindowValid();
    }

    void Window::CaptureCursorInternal(bool doCapture)
    {
        if (doCapture)
        {
            if (_pWindowState == nullptr)
                return;

            HWND hWindow = static_cast<HWND>(_pWindowState->hWindow);

            RECT rect;
            ::GetClientRect(hWindow, &rect);
            ::MapWindowPoints(hWindow, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
            ::ClipCursor(&rect);
        }
        else
        {
            ::ClipCursor(nullptr);
        }
    }

    bool Window::CalculateMouseInsideWindow() const
    {
        if (_pWindowState == nullptr)
            return false;

        POINT cursorPos;
        if (!::GetCursorPos(&cursorPos))
            return false;

        const HWND hWnd = static_cast<HWND>(_pWindowState->hWindow);
        POINT clientPoint = cursorPos;
        ::ScreenToClient(hWnd, &clientPoint);

        RECT clientRect;
        ::GetClientRect(hWnd, &clientRect);

        return (clientPoint.x >= 0 && clientPoint.x < clientRect.right &&
            clientPoint.y >= 0 && clientPoint.y < clientRect.bottom);
    }

    int Window::WindowEventProcess(uint32_t message, void* wpara, void* lpara)
    {
        if (_pWindowState == nullptr)
            return 0;

        int ret;
        bool blockProcess = NativeWindowEventPreProcess(message, wpara, lpara, &ret);
        if (blockProcess)
            return ret;

        // WM_CLOSE in DefWindowProcW will cause WM_DESTROY sent.
        if (message == WM_CLOSE)
        {
            OnWindowClose();
            return 0;
        }

        // Hack the menu system command, so that pressing ALT or F10 doesn't steal the focus
        if ((message == WM_SYSCOMMAND) && (reinterpret_cast<WPARAM>(wpara) == SC_KEYMENU))
            return 0;

        WindowEventProcessInternal(message, wpara, lpara);

        if (message == WM_DESTROY)
            OnWindowPreDestroy();

        ret = ::DefWindowProcW(static_cast<HWND>(_pWindowState->hWindow)
            , message
            , reinterpret_cast<WPARAM>(wpara)
            , reinterpret_cast<LPARAM>(lpara));

        if (message == WM_DESTROY)
            OnWindowPostDestroy();

        return ret;
    }

    void Window::WindowEventProcessInternal(uint32_t message, void* wpara, void* lpara)
    {
        if (_pWindowState == nullptr)
            return;

        WPARAM wParam = reinterpret_cast<WPARAM>(wpara);
        LPARAM lParam = reinterpret_cast<LPARAM>(lpara);

        switch (message)
        {
            case WM_SETCURSOR:
            {
                // lower world of lParam is hit test result
                if (LOWORD(lParam) == HTCLIENT)
                    ::SetCursor(_pWindowState->cursorVisible ? static_cast<HCURSOR>(_pWindowState->hCursor) : nullptr);

                break;
            }
            case WM_SIZE:
            {
                auto newSize = GetSize();
                if (wParam != SIZE_MINIMIZED && (_pWindowState->width != newSize.first || _pWindowState->height != newSize.second))
                {
                    _pWindowState->width = newSize.first;
                    _pWindowState->height = newSize.second;
                    OnWindowResize(_pWindowState->width, _pWindowState->height);
                }
                break;
            }
            case WM_SETFOCUS:
            {
                CaptureCursorInternal(_pWindowState->cursorCapture);
                OnWindowGetFocus();
                break;
            }
            case WM_KILLFOCUS:
            {
                CaptureCursorInternal(false);
                OnWindowLostFocus();
                break;
            }

            case WM_MOUSEMOVE:
            {
                const HWND hWnd = static_cast<HWND>(_pWindowState->hWindow);
                const int x = static_cast<int16_t>(LOWORD(lParam));
                const int y = static_cast<int16_t>(HIWORD(lParam));

                RECT area;
                ::GetClientRect(hWnd, &area);

                // Capture the mouse in case the user wants to drag it outside
                if ((wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0)
                {
                    if (::GetCapture() == hWnd)
                        ::ReleaseCapture();
                }
                else if (::GetCapture() != hWnd)
                {
                    ::SetCapture(hWnd);
                }

                // Mouse is out of window
                if ((x < area.left) || (x > area.right) || (y < area.top) || (y > area.bottom))
                {
                    if (_pWindowState->mouseInsideWindow)
                    {
                        _pWindowState->mouseInsideWindow = false;
                        OnMouseEnterWindow();
                    }
                }
                else
                {
                    if (!_pWindowState->mouseInsideWindow)
                    {
                        _pWindowState->mouseInsideWindow = true;
                        OnMouseLeaveWindow();
                    }
                }
                break;
            }
        }

    }

}
