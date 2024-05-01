#include <algorithm>
#include <Helper/WinApi/WindowsPlatform.h>
#include <Helper/WinApi/WinApiResource.h>
#include <Helper/String.h>
#include "WinWindow.h"
#include "FontResource.h"

namespace ImGuiApp
{
    bool Win32Window::SetUp()
    {
        _width = GetWindowInitWidth();
        _height = GetWindowInitHeight();
        _windowRegisterName = Util::StringToWideString(GetWindowRegisterName());
        _windowTitle = Util::StringToWideString(GetWindowTitle());

        Win32RegisterWindow();
        Win32CreateWindow();

        if (!D3dCreateDevice())
            return false;

        ImGuiCreateRender();

        return true;
    }

    void Win32Window::Destroy()
    {
        ImGuiDestroyRender();
        D3dDestroyDevice();
        Win32DestroyWindow();
        Win32UnRegisterWindow();
    }

    void Win32Window::Show()
    {
        ::ShowWindow(_hWnd, SW_SHOWDEFAULT);
        ::UpdateWindow(_hWnd);
    }

    void Win32Window::WinMessageLoop(bool* isQuit)
    {
        MSG msg;

        if (BlockWhenNoWindowsMessage())
        {
            // When GetMessage() retrieves a WM_QUIT message from the queue, it will return a value of 0
            if (::GetMessage(&msg, nullptr, 0U, 0U) == 0)
            {
                *isQuit = true;
            }
            else
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
        else
        {
            while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);

                *isQuit = msg.message == WM_QUIT;
                if (*isQuit)
                    break;
            }
        }
    }

    void Win32Window::RenderLoop()
    {
        _pImGuiRender->NewFrame();

        std::for_each(_imGuiLogicVec.begin(), _imGuiLogicVec.end(), [](ImGuiLogic* pUpdater)
        {
            if (pUpdater != nullptr)
                pUpdater->Loop();
        });

        _pImGuiRender->EndFrame();

        ClearColor();
        _pImGuiRender->RenderDrawData();
        SwapChain();
    }

    void Win32Window::ClearColor()
    {
        const float clear_color_with_alpha[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
        _pD3dDeviceContext->OMSetRenderTargets(1, &_pMainRenderTarget, nullptr);
        _pD3dDeviceContext->ClearRenderTargetView(_pMainRenderTarget, clear_color_with_alpha);
    }

    void Win32Window::SwapChain()
    {
        _pSwapChain->Present(1, 0);
    }

    HWND Win32Window::GetWindowHandle() const
    {
        return _hWnd;
    }

    ID3D11Device* Win32Window::GetD3dDevice() const
    {
        return _pD3dDevice;
    }

    ID3D11DeviceContext* Win32Window::GetD3dDeviceContext() const
    {
        return _pD3dDeviceContext;
    }

    ImGuiRender* Win32Window::GetRender() const
    {
        return _pImGuiRender;
    }

    const char* Win32Window::GetWindowRegisterName()
    {
        return "MainWindow";
    }

    bool Win32Window::WindowHasMaxBox()
    {
        return true;
    }

    bool Win32Window::WindowHasMinBox()
    {
        return true;
    }

    bool Win32Window::WindowCanThickFrame()
    {
        return true;
    }

    int Win32Window::GetWindowIconResourceId()
    {
        return 0;
    }

    bool Win32Window::BlockWhenNoWindowsMessage()
    {
        return false;
    }

    LRESULT Win32Window::WndProcDispatch(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Win32Window* pThis = nullptr;
        if (msg == WM_CREATE)
        {
            auto longPointerCreateStructure = reinterpret_cast<LPCREATESTRUCTA>(lParam);
            pThis = static_cast<Win32Window*>(longPointerCreateStructure->lpCreateParams);
            ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        }
        else
        {
            pThis = reinterpret_cast<Win32Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
        }

        if (pThis != nullptr)
            return pThis->OnWinMsg(hWnd, msg, wParam, lParam);
        else
            return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }

    void Win32Window::AddWinMsgProc(IWinMsgReceiver* pWinMsgReceiver)
    {
        auto itr = std::find(_winMsgReceiverVec.begin(), _winMsgReceiverVec.end(), pWinMsgReceiver);
        if (itr == _winMsgReceiverVec.end())
            _winMsgReceiverVec.push_back(pWinMsgReceiver);
    }

    void Win32Window::RemoveWinMsgProc(IWinMsgReceiver* pWinMsgReceiver)
    {
        auto itr = std::find(_winMsgReceiverVec.begin(), _winMsgReceiverVec.end(), pWinMsgReceiver);
        if (itr != _winMsgReceiverVec.end())
            _winMsgReceiverVec.erase(itr);
    }

    void Win32Window::AddImGuiLogicUpdater(ImGuiLogic* pUpdater)
    {
        auto itr = std::find(_imGuiLogicVec.begin(), _imGuiLogicVec.end(), pUpdater);
        if (itr == _imGuiLogicVec.end())
            _imGuiLogicVec.push_back(pUpdater);
    }

    void Win32Window::RemoveImGuiLogicUpdater(ImGuiLogic* pUpdater)
    {
        auto itr = std::find(_imGuiLogicVec.begin(), _imGuiLogicVec.end(), pUpdater);
        if (itr != _imGuiLogicVec.end())
            _imGuiLogicVec.erase(itr);
    }

    LRESULT Win32Window::OnWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (!_winMsgReceiverVec.empty())
        {
            for (auto pWinMsgReceiver : _winMsgReceiverVec)
            {
                if (pWinMsgReceiver != nullptr)
                    pWinMsgReceiver->OnWinMsg(
                            reinterpret_cast<int64_t>(hWnd),
                            static_cast<uint32_t>(msg),
                            static_cast<int64_t>(wParam),
                            static_cast<int64_t>(lParam));
            }
        }

        switch (msg)
        {
            case WM_SIZE:
                return OnWinMsgSize(hWnd, msg, wParam, lParam);
            case WM_SYSCOMMAND:
                return OnWinMsgSysCommand(hWnd, msg, wParam, lParam);
            case WM_DESTROY:
                return OnWinMsgDestroy(hWnd, msg, wParam, lParam);
            default:
                return ::DefWindowProcW(hWnd, msg, wParam, lParam);
        }
    }

    LRESULT Win32Window::OnWinMsgSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (wParam == SIZE_MINIMIZED)
            return FALSE;

        int newWidth = (int)LOWORD(lParam);
        int newHeight = (int)HIWORD(lParam);

        if (_width != newWidth || _height != newHeight)
        {
            _width = newWidth;
            _height = newHeight;

            D3dDestroyRenderTarget();
            _pSwapChain->ResizeBuffers(0, _width, _height, DXGI_FORMAT_UNKNOWN, 0);
            D3dCreateRenderTarget();
        }

        return ::DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    LRESULT Win32Window::OnWinMsgSysCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return FALSE;

        return ::DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    LRESULT Win32Window::OnWinMsgDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        ::PostQuitMessage(0);
        return FALSE;
    }

    void Win32Window::Win32RegisterWindow()
    {
        int iconId = GetWindowIconResourceId();
        HICON hIcon = nullptr;
        if (iconId != 0)
        {
            auto loadIcon = Helper::Win::Resource::LoadResource<Helper::Win::Resource::IconResource>(iconId);
            hIcon = loadIcon.has_value() ? static_cast<HICON>(loadIcon.value().hIcon) : nullptr;
        }

        WNDCLASSEXW wc = {
                sizeof(wc),
                CS_CLASSDC,
                WndProcDispatch,
                0L,
                0L,
                ::GetModuleHandle(nullptr),
                hIcon,
                nullptr,
                nullptr,
                nullptr,
                _windowRegisterName.c_str(),
                hIcon
        };

        ::RegisterClassExW(&wc);
    }

    void Win32Window::Win32CreateWindow()
    {
        DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

        if (WindowHasMaxBox())
            windowStyle |= WS_MAXIMIZEBOX;

        if (WindowHasMinBox())
            windowStyle |= WS_MINIMIZEBOX;

        if (WindowCanThickFrame())
            windowStyle |= WS_THICKFRAME;

        // TODO: Pop up window & drag & close
        // WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP

        _hWnd = ::CreateWindowW(
                _windowRegisterName.c_str(),
                _windowTitle.c_str(),
                windowStyle,
                100,
                100,
                _width,
                _height,
                nullptr,
                nullptr,
                ::GetModuleHandle(nullptr),
                this);

        ::SetWindowTextW(_hWnd, _windowTitle.c_str());
    }

    void Win32Window::Win32DestroyWindow()
    {
        ::DestroyWindow(_hWnd);
    }

    void Win32Window::Win32UnRegisterWindow()
    {
        ::UnregisterClassW(_windowRegisterName.c_str(), GetModuleHandle(nullptr));
    }

    bool Win32Window::D3dCreateDevice()
    {
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = _hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pD3dDevice, &featureLevel, &_pD3dDeviceContext);
        if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
            res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pD3dDevice, &featureLevel, &_pD3dDeviceContext);
        if (res != S_OK)
            return false;

        D3dCreateRenderTarget();
        return true;
    }

    void Win32Window::D3dCreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        _pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        _pD3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pMainRenderTarget);
        pBackBuffer->Release();
    }

    void Win32Window::D3dDestroyDevice()
    {
        D3dDestroyRenderTarget();

        if (_pSwapChain)
        {
            _pSwapChain->Release();
            _pSwapChain = nullptr;
        }

        if (_pD3dDeviceContext)
        {
            _pD3dDeviceContext->Release();
            _pD3dDeviceContext = nullptr;
        }

        if (_pD3dDevice)
        {
            _pD3dDevice->Release();
            _pD3dDevice = nullptr;
        }
    }

    void Win32Window::D3dDestroyRenderTarget()
    {
        if (_pMainRenderTarget)
        {
            _pMainRenderTarget->Release();
            _pMainRenderTarget = nullptr;
        }
    }

    void Win32Window::ImGuiCreateRender()
    {
        _pImGuiRender = new ImGuiRender();
        _pImGuiRender->SetUp(_hWnd, _pD3dDevice, _pD3dDeviceContext);
        AddWinMsgProc(_pImGuiRender);
    }

    void Win32Window::ImGuiDestroyRender()
    {
        if (_pImGuiRender != nullptr)
        {
            RemoveWinMsgProc(_pImGuiRender);
            _pImGuiRender->Destroy();
            delete _pImGuiRender;
        }
    }
}
