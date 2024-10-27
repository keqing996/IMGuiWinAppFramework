#include "ImApp/Utility/WindowsInclude.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include "ImApp/ImGuiWinApp.h"
#include "ImApp/Theme/Spectrum.h"
#include <locale>
#include "ImApp/Font/SansProRegular.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ImApp
{
    ImGuiWinApp::ImGuiWinApp()
    {
        std::locale::global(std::locale("zh_CN.UTF8"));
    }

    bool ImGuiWinApp::Create(int width, int height, const std::string& title)
    {
        return Create(width, height, title, Backend::D3d11);
    }

    bool ImGuiWinApp::Create(int width, int height, const std::string& title, Backend backend)
    {
        return Create(width, height, title, backend, NativeWindow::WindowStyle::DefaultStyle());
    }

    bool ImGuiWinApp::Create(int width, int height, const std::string& title, NativeWindow::WindowStyle style)
    {
        return Create(width, height, title, Backend::D3d11, style);
    }

    bool ImGuiWinApp::Create(int width, int height, const std::string& title, Backend backend, NativeWindow::WindowStyle style)
    {
        bool result = Window::Create(width, height, title, style);
        if (!result)
            return false;

        _pBackend = ImGuiBackend::Create(this, backend);
        _pBackend->SetupDevice();

        ImGuiInitConfig();
        ImGuiInitFrontend();
        ImGuiInitBackend();

        InitTheme();

        OnImGuiInitialized();

        return true;
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
        ImGui_ImplWin32_Init(GetSystemHandle());
    }

    void ImGuiWinApp::ImGuiInitBackend()
    {
        _pBackend->SetupImGui();
    }

    void ImGuiWinApp::InitTheme()
    {
        float dpiScale =  GetDpiScale();

        // Font
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        io.FontDefault = CreateImGuiFont(
            SourceSansProRegular.data(),
            SourceSansProRegular.size(),
            GetDefaultFontSize(),
            false);

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
            bool finish;
            EventLoop(&finish);

            if (finish)
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

    std::optional<Backend> ImGuiWinApp::GetBackendType() const
    {
        if (_pBackend == nullptr)
            return std::nullopt;

        return _pBackend->GetBackendType();
    }

    void ImGuiWinApp::OnWindowCreated()
    {
        Window::OnWindowCreated();
    }

    bool ImGuiWinApp::NativeWindowEventPreProcess(uint32_t message, void* wpara, void* lpara, int* result)
    {
        *result = ImGui_ImplWin32_WndProcHandler(static_cast<HWND>(GetSystemHandle()), message,
            reinterpret_cast<LPARAM>(wpara), reinterpret_cast<WPARAM>(lpara));
        return false;
    }

    void ImGuiWinApp::OnWindowClose()
    {
        Window::OnWindowClose();
    }

    void ImGuiWinApp::OnWindowPreDestroy()
    {
        _pBackend->ClearImGui();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        _pBackend->ClearDevice();

        Window::OnWindowPreDestroy();
    }

    void ImGuiWinApp::OnWindowPostDestroy()
    {
        Window::OnWindowPostDestroy();
    }

    void ImGuiWinApp::OnWindowResize(int width, int height)
    {
        Window::OnWindowResize(width, height);
    }

    void ImGuiWinApp::OnWindowGetFocus()
    {
        Window::OnWindowGetFocus();
    }

    void ImGuiWinApp::OnWindowLostFocus()
    {
        Window::OnWindowLostFocus();
    }

    void ImGuiWinApp::OnMouseEnterWindow()
    {
        Window::OnMouseEnterWindow();
    }

    void ImGuiWinApp::OnMouseLeaveWindow()
    {
        Window::OnMouseLeaveWindow();
    }

    void ImGuiWinApp::OnImGuiInitialized()
    {
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
        return ImGui_ImplWin32_GetDpiScaleForHwnd(GetSystemHandle());
    }

    int ImGuiWinApp::GetDefaultFontSize()
    {
        return 16 * GetDpiScale();
    }

    ImFont* ImGuiWinApp::CreateImGuiFont(void* fontData, int fontDataSize, int fontSize, bool transferDataOwnership)
    {
        HWND hWnd = static_cast<HWND>(GetSystemHandle());
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
        HWND hWnd = static_cast<HWND>(GetSystemHandle());
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
        HWND hWnd = static_cast<HWND>(GetSystemHandle());
        ::PostMessageW(hWnd, WM_CLOSE, 0, 0);
    }

}