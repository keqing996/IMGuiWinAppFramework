#include "ImApp/Utility/WindowsInclude.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include "ImApp/ImGuiWinApp.h"
#include "ImApp/Font/JetBrainsMono-Bold.h"
#include "ImApp/Font/JetBrainsMono-Regular.h"

#pragma comment(lib, "d3d11.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool ImGui_ImplWin32_WndProcHandler_Wrapper(void* hWnd, uint32_t msg, void* lParam, void* wParam)
{
    return ImGui_ImplWin32_WndProcHandler((HWND)hWnd, msg, (LPARAM)lParam, (WPARAM)wParam);
}

namespace IMWinApp
{
    ImGuiWinApp::ImGuiWinApp(int width, int height, const std::string& title, int style)
        : _window(width, height, title, style)
    {
        _pBackend->SetupDevice(_window.GetSystemHandle());

        ImGuiInitConfig();
        ImGuiInitFrontend();
        ImGuiInitBackend();
        ImGuiInitFont();

        _window.SetWindowEventProcessFunction(ImGui_ImplWin32_WndProcHandler_Wrapper);
    }

    void ImGuiWinApp::D3d11Clear()
    {
        if (_pMainRenderTargetView)
        {
            _pMainRenderTargetView->Release();
            _pMainRenderTargetView = nullptr;
        }

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

    ImGuiWinApp::~ImGuiWinApp()
    {
        ImGuiClear();
        D3d11Clear();
    }

    void ImGuiWinApp::ImGuiInitConfig()
    {
        IMGUI_CHECKVERSION();

        // Prepare font atlas
        _pSharedImGuiFonts = std::make_unique<ImFontAtlas>();

        // ImGui context
        ImGui::CreateContext(_pSharedImGuiFonts.get());
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Light style
        ImGui::StyleColorsLight();

        // Disable imgui ini file
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
    }

    void ImGuiWinApp::ImGuiInitFrontend()
    {
        ImGui_ImplWin32_Init((HWND)_window.GetSystemHandle());
    }

    void ImGuiWinApp::ImGuiInitBackend()
    {
        _pBackend->SetupImGui();
    }

    void ImGuiWinApp::ImGuiInitFont()
    {
        // Scale
        HWND hWnd = (HWND)_window.GetSystemHandle();
        float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(reinterpret_cast<HWND>(hWnd));
        ImGui::GetStyle().ScaleAllSizes(dpiScale);

        // Font
        _pFontRegularNormal = CreateImGuiFont(JetBrainsMono_Regular.data(),JetBrainsMono_Regular.size(), NORMAL_FONT_SIZE, false);
        _pFontRegularLarge = CreateImGuiFont(JetBrainsMono_Regular.data(),JetBrainsMono_Regular.size(), LARGE_FONT_SIZE, false);
        _pFontBoldNormal = CreateImGuiFont(JetBrainsMono_Bold.data(),JetBrainsMono_Bold.size(), NORMAL_FONT_SIZE, false);
        _pFontBoldLarge = CreateImGuiFont(JetBrainsMono_Bold.data(),JetBrainsMono_Bold.size(), LARGE_FONT_SIZE, false);
    }

    void ImGuiWinApp::ImGuiClear()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiWinApp::AppLoop()
    {
        while (true)
        {
            // Win32 message loop
            _window.EventLoop();

            bool breakAppLoop = false;
            while (_window.HasEvent())
            {
                auto allEvent = _window.PopAllEvent();

                for (auto& event: allEvent)
                {
                    if (_onEventHandler && _onEventHandler(event, &breakAppLoop))
                        continue;

                    DefaultOnEventHandler(event, &breakAppLoop);
                }
            }

            if (breakAppLoop)
                break;

            // Pre frame tick
            if (_preFrameTick)
                _preFrameTick(*this);

            // ImGui new frame setup
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            // On frame tick
            if (_onFrameTick)
                _onFrameTick(*this);

            // ImGui render
            ImGui::Render();

            // On frame tick
            if (_postFrameTick)
                _postFrameTick(*this);

            // Clear color
            static const float CLEAR_COLOR[4] = {0.75f, 0.75f, 0.75f, 1.00f };
            _pD3dDeviceContext->OMSetRenderTargets(1, &_pMainRenderTargetView, nullptr);
            _pD3dDeviceContext->ClearRenderTargetView(_pMainRenderTargetView, CLEAR_COLOR);

            // ImGui draw
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            // Swap back buffer
            if (_enableVSync)
                _pSwapChain->Present(1, 0);
            else
                _pSwapChain->Present(0, 0);
        }
    }

    void ImGuiWinApp::EnableVSync(bool enable)
    {
        _enableVSync = enable;
    }

    ImFont* ImGuiWinApp::GetFontRegularNormal() const
    {
        return _pFontRegularNormal;
    }

    ImFont* ImGuiWinApp::GetFontRegularLarge() const
    {
        return _pFontRegularLarge;
    }

    ImFont* ImGuiWinApp::GetFontBoldNormal() const
    {
        return _pFontBoldNormal;
    }

    ImFont* ImGuiWinApp::GetFontBoldLarge() const
    {
        return _pFontBoldLarge;
    }

    ImFont* ImGuiWinApp::CreateImGuiFont(void* fontData, int fontDataSize, int fontSize, bool transferDataOwnership)
    {
        HWND hWnd = (HWND)_window.GetSystemHandle();
        float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(reinterpret_cast<HWND>(hWnd));

        ImFontConfig tempConfig;
        if (!transferDataOwnership)
            tempConfig.FontDataOwnedByAtlas = false;

        return _pSharedImGuiFonts->AddFontFromMemoryTTF(
                fontData,
                fontDataSize,
                dpiScale * fontSize,
                &tempConfig,
                _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());
    }

    ImFont* ImGuiWinApp::CreateImGuiFont(const std::string& ttfPath, int fontSize)
    {
        HWND hWnd = (HWND)_window.GetSystemHandle();
        float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(reinterpret_cast<HWND>(hWnd));

        return _pSharedImGuiFonts->AddFontFromFileTTF(
                ttfPath.c_str(),
                dpiScale * fontSize,
                nullptr,
                _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());
    }

    void ImGuiWinApp::CloseWindow()
    {
        HWND hWnd = (HWND)_window.GetSystemHandle();
        ::PostMessageW(hWnd, WM_CLOSE, 0, 0);
    }

    void ImGuiWinApp::SetOnEventHandler(const std::function<bool(const NWA::WindowEvent&, bool*)>& handler)
    {
        _onEventHandler = handler;
    }

    void ImGuiWinApp::DefaultOnEventHandler(const NWA::WindowEvent& event, bool* breakAppLoop)
    {
        *breakAppLoop = event.type == NWA::WindowEvent::Type::Close;
    }

    NWA::Window& ImGuiWinApp::GetNativeWindow()
    {
        return _window;
    }

    int ImGuiWinApp::GetNormalFontSize()
    {
        return NORMAL_FONT_SIZE;
    }

    int ImGuiWinApp::GetLargeFontSize()
    {
        return LARGE_FONT_SIZE;
    }

}