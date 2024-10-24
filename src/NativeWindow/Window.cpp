
#include "ImApp/Utility/WindowsInclude.h"
#include "ImApp/Utility/String.h"
#include "ImApp/NativeWindow/Window.h"
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
        Clear();
    }

    bool Window::Initialize(int width, int height, const std::string& title)
    {
        WindowStyle style{};
        style.type = WindowStyle::Type::Overlapped;
        style.config.overlapped = WindowStyle::OverlappedStyle();
        return Initialize(width, height, title, style);
    }

    bool Window::Initialize(int width, int height, const std::string& title, WindowStyle style)
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

        // Set size again after window creation to avoid some bug.
        SetSize(width, height);

        OnWindowInitialized();

        return true;
    }

    void Window::Clear()
    {


        // Destroy window
        ::DestroyWindow(static_cast<HWND>(_pWindowState->hWindow));


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
        if (_hIcon != nullptr)
            ::DestroyIcon(static_cast<HICON>(_hIcon));

        _hIcon = nullptr;

        // Change RGBA to BGRA
        std::vector<std::byte> iconPixels(width * height * 4);
        for (std::size_t i = 0; i < iconPixels.size() / 4; ++i)
        {
            iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
            iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
            iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
            iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
        }

        _hIcon = ::CreateIcon(
                GetModuleHandleW(nullptr),
                static_cast<int>(width),
                static_cast<int>(height),
                1,
                32,
                nullptr,
                (unsigned char*)iconPixels.data());

        if (_hIcon != nullptr)
        {
            ::SendMessageW(
                    static_cast<HWND>(_hWindow),
                    WM_SETICON,
                    ICON_BIG,
                    reinterpret_cast<LPARAM>(_hIcon));

            ::SendMessageW(
                    static_cast<HWND>(_hWindow),
                    WM_SETICON,
                    ICON_SMALL,
                    reinterpret_cast<LPARAM>(_hIcon));
        }
    }

    void Window::SetIcon(int iconResId)
    {
        if (_hIcon != nullptr)
            ::DestroyIcon(static_cast<HICON>(_hIcon));

        _hIcon = nullptr;

        _hIcon = ::LoadIconW(GetModuleHandleW(nullptr), MAKEINTRESOURCE(iconResId));
        if (_hIcon != nullptr)
        {
            ::SendMessageW(
                    static_cast<HWND>(_hWindow),
                    WM_SETICON,
                    ICON_BIG,
                    reinterpret_cast<LPARAM>(_hIcon));

            ::SendMessageW(
                    static_cast<HWND>(_hWindow),
                    WM_SETICON,
                    ICON_SMALL,
                    reinterpret_cast<LPARAM>(_hIcon));
        }
    }

    void Window::SetWindowVisible(bool show)
    {
        ::ShowWindow(static_cast<HWND>(_hWindow), show ? SW_SHOW : SW_HIDE);
    }

    void Window::SetCursorVisible(bool show)
    {
        _cursorVisible = show;
        ::SetCursor(_cursorVisible ? static_cast<HCURSOR>(_hCursor) : nullptr);
    }

    void Window::SetCursorCapture(bool capture)
    {
        _cursorCapture = capture;
        CaptureCursorInternal(_cursorCapture);
    }

    bool Window::GetCursorVisible() const
    {
        return _cursorVisible;
    }

    bool Window::GetCursorCapture() const
    {
        return _cursorCapture;
    }

    bool Window::GetKeyRepeated() const
    {
        return _enableKeyRepeat;
    }

    void Window::SetKeyRepeated(bool repeated)
    {
        _enableKeyRepeat = repeated;
    }

    bool Window::WindowEventPreProcess(uint32_t message, void* wpara, void* lpara)
    {
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
        const auto titleInWideStr = Utility::StringToWideString(title);
        const wchar_t* titleWide = titleInWideStr.c_str();
        ::SetWindowTextW(static_cast<HWND>(_hWindow), titleWide);
    }

    std::pair<int, int> Window::GetSize()
    {
        RECT rect;
        ::GetClientRect(static_cast<HWND>(_hWindow), &rect);
        return { static_cast<int>(rect.right - rect.left), static_cast<int>(rect.bottom - rect.top) };
    }

    std::pair<int, int> Window::GetPosition()
    {
        RECT rect;
        ::GetWindowRect(static_cast<HWND>(_hWindow), &rect);

        return { static_cast<int>(rect.left), static_cast<int>(rect.top) };
    }

    void Window::SetPosition(int x, int y)
    {
        ::SetWindowPos(
                static_cast<HWND>(_hWindow),
                nullptr,
                x,
                y,
                0,
                0,
                SWP_NOSIZE | SWP_NOZORDER);

        // Adjust cursor position
        if(_cursorCapture)
            SetCursorCapture(true);
    }

    bool Window::EventLoop()
    {
        // Fetch new event
        MSG message;
        while (::PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&message);
            ::DispatchMessageW(&message);
        }

        if (_destroyMessageReceived)
        {
            _destroyMessageReceived = false;
            return false;
        }

        return true;
    }

    void Window::CaptureCursorInternal(bool doCapture)
    {
        if (doCapture)
        {
            RECT rect;
            ::GetClientRect(static_cast<HWND>(_hWindow), &rect);
            ::MapWindowPoints(static_cast<HWND>(_hWindow), nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
            ::ClipCursor(&rect);
        }
        else
        {
            ::ClipCursor(nullptr);
        }
    }

    void Window::SetWindowEventProcessFunction(const std::function<bool(void*, uint32_t, void*, void*)>& f)
    {
        _winEventProcess = f;
    }

    void Window::ClearWindowEventProcessFunction()
    {
        _winEventProcess = nullptr;
    }

    int Window::WindowEventProcess(uint32_t message, void* wpara, void* lpara)
    {
        bool handled = WindowEventPreProcess(message, wpara, lpara);
        if (handled)
            return;

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

        auto ret = ::DefWindowProcW(handle, message, wParam, lParam);

        if (message == WM_DESTROY)
            OnWindowPostDestroy();

        return ret;
    }

    void Window::WindowEventProcessInternal(uint32_t message, void* wpara, void* lpara)
    {
        if (_hWindow == nullptr)
            return;

        WPARAM wParam = reinterpret_cast<WPARAM>(wpara);
        LPARAM lParam = reinterpret_cast<LPARAM>(lpara);

        switch (message)
        {
            case WM_SETCURSOR:
            {
                // lower world of lParam is hit test result
                if (LOWORD(lParam) == HTCLIENT)
                    ::SetCursor(_cursorVisible ? static_cast<HCURSOR>(_hCursor) : nullptr);

                break;
            }
            case WM_SIZE:
            {
                auto newSize = GetSize();
                if (wParam != SIZE_MINIMIZED && _windowSize != newSize)
                {
                    _windowSize = newSize;
                    OnWindowResize(_windowSize.first, _windowSize.second);
                }
                break;
            }
            case WM_SETFOCUS:
            {
                CaptureCursorInternal(_cursorCapture);
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
                const HWND hWnd = static_cast<HWND>(_hWindow);
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
                    if (_mouseInsideWindow)
                    {
                        _mouseInsideWindow = false;
                        OnMouseEnterWindow();
                    }
                }
                else
                {
                    if (!_mouseInsideWindow)
                    {
                        _mouseInsideWindow = true;
                        OnMouseLeaveWindow();
                    }
                }
                break;
            }
        }

    }

}
