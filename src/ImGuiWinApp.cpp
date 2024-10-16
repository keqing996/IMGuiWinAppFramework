#include "ImApp/Utility/WindowsInclude.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include "ImApp/ImGuiWinApp.h"

#include <imgui_internal.h>
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

        ImGuiInitConfig();
        ImGuiInitFrontend();
        ImGuiInitBackend();
        ImGuiInitFont();

        InitLocale();
        InitStyle();

        _pWindow->SetWindowEventProcessFunction(ImGui_ImplWin32_WndProcHandler_Wrapper);
    }

    void ImGuiWinApp::ImGuiInitConfig()
    {
        IMGUI_CHECKVERSION();

        // Prepare font atlas
        _pSharedImGuiFonts = std::make_shared<ImFontAtlas>();

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
        ImGui_ImplWin32_Init((HWND)_pWindow->GetSystemHandle());
    }

    void ImGuiWinApp::ImGuiInitBackend()
    {
        _pBackend->SetupImGui();
    }

    void ImGuiWinApp::ImGuiInitFont()
    {
        // Scale
        HWND hWnd = (HWND)_pWindow->GetSystemHandle();
        float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(reinterpret_cast<HWND>(hWnd));
        ImGui::GetStyle().ScaleAllSizes(dpiScale);

        // Font
        _pFontRegularNormal = CreateImGuiFont(JetBrainsMono_Regular.data(),JetBrainsMono_Regular.size(), NORMAL_FONT_SIZE, false);
        _pFontRegularLarge = CreateImGuiFont(JetBrainsMono_Regular.data(),JetBrainsMono_Regular.size(), LARGE_FONT_SIZE, false);
        _pFontBoldNormal = CreateImGuiFont(JetBrainsMono_Bold.data(),JetBrainsMono_Bold.size(), NORMAL_FONT_SIZE, false);
        _pFontBoldLarge = CreateImGuiFont(JetBrainsMono_Bold.data(),JetBrainsMono_Bold.size(), LARGE_FONT_SIZE, false);
    }

    void ImGuiWinApp::InitLocale()
    {
        std::locale::global(std::locale("zh_CN.UTF8"));
    }

    void ImGuiWinApp::InitStyle()
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        colors[ImGuiCol_Text]                   = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
        colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
        colors[ImGuiCol_Button]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_TabHovered]             = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_Tab]                    = ImLerp(colors[ImGuiCol_Header],       colors[ImGuiCol_TitleBgActive], 0.90f);
        colors[ImGuiCol_TabSelected]            = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
        colors[ImGuiCol_TabSelectedOverline]    = colors[ImGuiCol_HeaderActive];
        colors[ImGuiCol_TabDimmed]              = ImLerp(colors[ImGuiCol_Tab],          colors[ImGuiCol_TitleBg], 0.80f);
        colors[ImGuiCol_TabDimmedSelected]      = ImLerp(colors[ImGuiCol_TabSelected],  colors[ImGuiCol_TitleBg], 0.40f);
        colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.26f, 0.59f, 1.00f, 1.00f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.78f, 0.87f, 0.98f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
        colors[ImGuiCol_TextLink]               = colors[ImGuiCol_HeaderActive];
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_NavHighlight]           = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
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

    void ImGuiWinApp::PreTick()
    {
    }

    void ImGuiWinApp::Tick()
    {
    }

    void ImGuiWinApp::PostTick()
    {
    }

    ImFont* ImGuiWinApp::CreateImGuiFont(void* fontData, int fontDataSize, int fontSize, bool transferDataOwnership)
    {
        HWND hWnd = static_cast<HWND>(_pWindow->GetSystemHandle());
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
        HWND hWnd = static_cast<HWND>(_pWindow->GetSystemHandle());
        float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(reinterpret_cast<HWND>(hWnd));

        return _pSharedImGuiFonts->AddFontFromFileTTF(
                ttfPath.c_str(),
                dpiScale * fontSize,
                nullptr,
                _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());
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