#include "ImApp/Utility/WindowsInclude.h"
#include "ImApp/NativeWindow/Window.h"

namespace NativeWindow
{
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
                // Do nothing for now.
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