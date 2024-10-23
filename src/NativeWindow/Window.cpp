
#include "ImApp/Utility/WindowsInclude.h"
#include "ImApp/Utility/String.h"
#include "ImApp/NativeWindow/Window.h"
#include "NativeWindowUtility.h"

namespace NativeWindow
{
    static int gGlobalWindowsCount = 0;
    static const wchar_t* gWindowRegisterName = L"InfraWindow";

    Window::Window(int width, int height, const std::string& title)
        : Window(width, height, title, static_cast<int>(WindowStyle::Default))
    {
    }

    Window::Window(int width, int height, const std::string& title, int style)
        : _hWindow(nullptr)
        , _hDeviceHandle(nullptr)
        , _windowSize({width, height})
        , _enableKeyRepeat(true)
        , _cursorVisible(true)
        , _cursorCapture(false)
        , _mouseInsideWindow(false)
        , _hIcon(nullptr)
        , _hCursor(::LoadCursor(nullptr, IDC_ARROW))
    {
        // Fix dpi
        NativeWindowUtility::FixProcessDpi();

        // Register window
        if (gGlobalWindowsCount == 0)
            RegisterWindowClass();

        // Get create style
        DWORD win32Style = WS_VISIBLE;
        if (style == static_cast<int>(WindowStyle::None))
            win32Style |= WS_POPUP;
        else
        {
            if (style & static_cast<int>(WindowStyle::HaveTitleBar))
                win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
            if (style & static_cast<int>(WindowStyle::HaveResize))
                win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
            if (style & static_cast<int>(WindowStyle::HaveClose))
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
        _hWindow = ::CreateWindowW(
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

        if (!_hWindow)
            return;

        // Get device context
        _hDeviceHandle = ::GetDC(static_cast<HWND>(_hWindow));

        // Global counting
        gGlobalWindowsCount++;

        // Set size again after window creation to avoid some bug.
        SetSize(width, height);
    }

    Window::~Window()
    {
        SetCursorVisible(true);
        ::ReleaseCapture();

        if (_hDeviceHandle)
            ::ReleaseDC(static_cast<HWND>(_hWindow), static_cast<HDC>(_hDeviceHandle));

        // Icon
        if (_hIcon != nullptr)
            ::DestroyIcon(static_cast<HICON>(_hIcon));

        // Destroy window
        ::DestroyWindow(static_cast<HWND>(_hWindow));

        // Global counting
        gGlobalWindowsCount--;

        if (gGlobalWindowsCount == 0)
            UnRegisterWindowClass();
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
        const HWND hWnd = static_cast<HWND>(_hWindow);
        const DWORD dwStyle = static_cast<DWORD>(::GetWindowLongPtrW(hWnd, GWL_STYLE));
        auto [adjustWidth, adjustHeight] = NativeWindowUtility::CalculateAdjustWindowSize(width, height, dwStyle);
        ::SetWindowPos(hWnd, nullptr, 0, 0, adjustWidth, adjustHeight, SWP_NOMOVE | SWP_NOZORDER);
    }

    void* Window::GetSystemHandle() const
    {
        return _hWindow;
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
        ::DestroyWindow((HWND)_hWindow);
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

    void Window::WindowEventProcess(uint32_t message, void* wpara, void* lpara)
    {
        bool handled = WindowEventPreProcess(message, wpara, lpara);
        if (handled)
            return;

        WindowEventProcessInternal(message, wpara, lpara);
    }

    void Window::WindowEventProcessInternal(uint32_t message, void* wpara, void* lpara)
    {
        if (_hWindow == nullptr)
            return;

        WPARAM wParam = reinterpret_cast<WPARAM>(wpara);
        LPARAM lParam = reinterpret_cast<LPARAM>(lpara);

        switch (message)
        {
            case WM_DESTROY:
            {
                _destroyMessageReceived = true;
                break;
            }
            case WM_CLOSE:
            {
                OnWindowClose();
                break;
            }
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
