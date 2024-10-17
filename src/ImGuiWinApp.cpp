#include "ImApp/Utility/WindowsInclude.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include "ImApp/ImGuiWinApp.h"
#include "ImApp/Theme/Spectrum.h"
#include <locale>

#include "ImApp/Font/JetBrainsMono-Bold.h"
#include "ImApp/Font/JetBrainsMono-Regular.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool ImGui_ImplWin32_WndProcHandler_Wrapper(void* hWnd, uint32_t msg, void* lParam, void* wParam)
{
    return ImGui_ImplWin32_WndProcHandler((HWND)hWnd, msg, (LPARAM)lParam, (WPARAM)wParam);
}

static void DefaultOnEventHandler(const NativeWindow::WindowEvent& event, bool* breakAppLoop)
{
    *breakAppLoop = event.type == NativeWindow::WindowEvent::Type::Close;
}

namespace ImApp
{
    ImGuiWinApp::~ImGuiWinApp()
    {
        _pBackend->ClearImGui();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        _pBackend->ClearDevice();
    }

    void ImGuiWinApp::InitWindow(int width, int height, const std::string& title, int style, Backend backend)
    {
        _pWindow = std::make_unique<NativeWindow::Window>(width, height, title, style);

        _pBackend = ImGuiBackend::Create(_pWindow.get(), backend);
        _pBackend->SetupDevice();

        InitLocale();

        ImGuiInitConfig();
        ImGuiInitFrontend();
        ImGuiInitBackend();

        InitTheme();

        _pWindow->SetWindowEventProcessFunction(ImGui_ImplWin32_WndProcHandler_Wrapper);
    }

    void ImGuiWinApp::ImGuiInitConfig()
    {
        IMGUI_CHECKVERSION();

        // ImGui context
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
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
        ImGui_ImplWin32_Init((HWND)_pWindow->GetSystemHandle());
    }

    void ImGuiWinApp::ImGuiInitBackend()
    {
        _pBackend->SetupImGui();
    }

    void ImGuiWinApp::InitLocale()
    {
        std::locale::global(std::locale("zh_CN.UTF8"));
    }

    void ImGuiWinApp::InitTheme()
    {
        float dpiScale =  GetDpiScale();

        // Font
        ImGui::GetIO().Fonts->Clear();
        Spectrum::LoadFont(NORMAL_FONT_SIZE, dpiScale);

        // Style
        Spectrum::LoadStyle(false);

        // Dpi scale
        ImGui::GetStyle().ScaleAllSizes(dpiScale);
    }

    void ImGuiWinApp::Loop()
    {
        while (true)
        {
            // Win32 message loop
            _pWindow->EventLoop();

            bool breakAppLoop = false;
            while (_pWindow->HasEvent())
            {
                auto allEvent = _pWindow->PopAllEvent();

                for (auto& event: allEvent)
                {
                    if (_onEventHandler && _onEventHandler(event, &breakAppLoop))
                        continue;

                    DefaultOnEventHandler(event, &breakAppLoop);
                }
            }

            if (breakAppLoop)
                break;

            // ImGui new frame setup
            _pBackend->NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            // Tick
            PreTick();
            Tick();
            PostTick();

            // ImGui render
            ImGui::Render();

            // Draw
            _pBackend->Draw();

            // Swap back buffer
            _pBackend->SwapBuffer();
        }
    }

    void ImGuiWinApp::SetVSyncEnable(bool enable)
    {
        _pBackend->SetVSyncEnable(enable);
    }

    bool ImGuiWinApp::GetVSyncEnable() const
    {
        return _pBackend->GetVSyncEnable();
    }

    void ImGuiWinApp::SetClearColor(const Utility::Color& color)
    {
        _pBackend->SetClearColor(color);
    }

    void ImGuiWinApp::PreTick()
    {
    }

    void ImGuiWinApp::Tick()
    {
    }

    void ImGuiWinApp::PostTick()
    {
    }

    float ImGuiWinApp::GetDpiScale()
    {
        HWND hWnd = (HWND)_pWindow->GetSystemHandle();
        float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(reinterpret_cast<HWND>(hWnd));
        return dpiScale;
    }

    ImFont* ImGuiWinApp::CreateImGuiFont(void* fontData, int fontDataSize, int fontSize, bool transferDataOwnership)
    {
        HWND hWnd = static_cast<HWND>(_pWindow->GetSystemHandle());
        float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(hWnd);

        ImFontConfig tempConfig;
        if (!transferDataOwnership)
            tempConfig.FontDataOwnedByAtlas = false;

        auto pFonts = ImGui::GetIO().Fonts;
        return pFonts->AddFontFromMemoryTTF(
                fontData,
                fontDataSize,
                dpiScale * fontSize,
                &tempConfig,
                pFonts->GetGlyphRangesChineseSimplifiedCommon());
    }

    ImFont* ImGuiWinApp::CreateImGuiFont(const std::string& ttfPath, int fontSize)
    {
        HWND hWnd = static_cast<HWND>(_pWindow->GetSystemHandle());
        float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(reinterpret_cast<HWND>(hWnd));

        auto pFonts = ImGui::GetIO().Fonts;
        return pFonts->AddFontFromFileTTF(
                ttfPath.c_str(),
                dpiScale * fontSize,
                nullptr,
                pFonts->GetGlyphRangesChineseSimplifiedCommon());
    }

    void ImGuiWinApp::CloseWindow()
    {
        HWND hWnd = static_cast<HWND>(_pWindow->GetSystemHandle());
        ::PostMessageW(hWnd, WM_CLOSE, 0, 0);
    }

    void ImGuiWinApp::SetOnEventHandler(const std::function<bool(const NativeWindow::WindowEvent&, bool*)>& handler)
    {
        _onEventHandler = handler;
    }

    NativeWindow::Window* ImGuiWinApp::GetNativeWindow()
    {
        return _pWindow.get();
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