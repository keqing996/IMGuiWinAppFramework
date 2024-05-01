#include <cstdint>
#include "ImGuiRender.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_win32.h"
#include "ImGui/backends/imgui_impl_dx11.h"
#include "FontResource.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ImGuiApp
{
    void ImGuiRender::InitSharedFontAtlas()
    {
        if (_pSharedImGuiFonts != nullptr)
            return;

        _pSharedImGuiFonts = new ImFontAtlas();
    }

    ImGuiRender::ImGuiRender()
    {
    }

    ImGuiRender::~ImGuiRender()
    {
    }

    void ImGuiRender::SetUp(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    {
        IMGUI_CHECKVERSION();

        InitSharedFontAtlas();

        ImGui::CreateContext(_pSharedImGuiFonts);
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

        // close imGui ini file generate
        io.IniFilename = nullptr;

        // color light
        ImGui::StyleColorsLight();

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(pDevice, pDeviceContext);

        // Scale
        float dpiScale = GetDpiScale(reinterpret_cast<int64_t>(hWnd));
        ImGui::GetStyle().ScaleAllSizes(dpiScale);

        // Font
        auto LoadMemoryFont = [this, dpiScale](int resId, int size) -> ImFont*
        {
            auto defaultFontRes = Helper::Win::Resource::LoadResource<Helper::Win::Resource::DataResource>(resId);
            if (!defaultFontRes.has_value())
                return nullptr;

            return _pSharedImGuiFonts->AddFontFromMemoryTTF(
                    defaultFontRes.value().data,
                    defaultFontRes.value().size,
                    dpiScale * size,
                    nullptr,
                    _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());
        };

        auto LoadMemoryFromFile = [this, dpiScale](const char* fileName, int size) -> ImFont*
        {
            return _pSharedImGuiFonts->AddFontFromFileTTF(
                    fileName,
                    dpiScale * size,
                    nullptr,
                    _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());
        };

#ifdef EMBEDDED_TTF

        _pFontRegularNormal = LoadMemoryFont(RES_TTF_REGULAR, NORMAL_FONT_SIZE);
        _pFontRegularLarge = LoadMemoryFont(RES_TTF_REGULAR, LARGE_FONT_SIZE);
        _pFontBoldNormal = LoadMemoryFont(RES_TTF_BOLD, NORMAL_FONT_SIZE);
        _pFontBoldLarge = LoadMemoryFont(RES_TTF_BOLD, LARGE_FONT_SIZE);

#endif

        static constexpr const char* SYSTEM_MSYH_REGULAR_FONT_PATH = "c:\\Windows\\Fonts\\msyhl.ttc";
        static constexpr const char* SYSTEM_MSYH_BOLD_FONT_PATH = "c:\\Windows\\Fonts\\msyhbd.ttc";

        if (_pFontRegularNormal == nullptr)
            _pFontRegularNormal = LoadMemoryFromFile(SYSTEM_MSYH_REGULAR_FONT_PATH, NORMAL_FONT_SIZE);

        if (_pFontRegularLarge == nullptr)
            _pFontRegularLarge = LoadMemoryFromFile(SYSTEM_MSYH_REGULAR_FONT_PATH, LARGE_FONT_SIZE);

        if (_pFontBoldNormal == nullptr)
            _pFontBoldNormal = LoadMemoryFromFile(SYSTEM_MSYH_BOLD_FONT_PATH, NORMAL_FONT_SIZE);

        if (_pFontBoldLarge == nullptr)
            _pFontBoldLarge = LoadMemoryFromFile(SYSTEM_MSYH_BOLD_FONT_PATH, LARGE_FONT_SIZE);

    }

    void ImGuiRender::Destroy()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiRender::NewFrame()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRender::EndFrame()
    {
        ImGui::Render();
    }

    void ImGuiRender::RenderDrawData()
    {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiRender::OnWinMsg(int64_t hWnd, uint32_t msg, int64_t wParam, int64_t lParam)
    {
        ImGui_ImplWin32_WndProcHandler(
                reinterpret_cast<HWND>(hWnd),
                static_cast<UINT>(msg),
                static_cast<WPARAM>(wParam),
                static_cast<LPARAM>(lParam));
    }

    float ImGuiRender::GetDpiScale(int64_t hWnd) const
    {
        return ImGui_ImplWin32_GetDpiScaleForHwnd(reinterpret_cast<HWND>(hWnd));
    }

    ImFont* ImGuiRender::GetRegularFontNormal() const
    {
        return _pFontRegularNormal;
    }

    ImFont* ImGuiRender::GetRegularFontLarge() const
    {
        return _pFontRegularLarge;
    }

    ImFont* ImGuiRender::GetBoldFontNormal() const
    {
        return _pFontBoldNormal;
    }

    ImFont* ImGuiRender::GetBoldFontLarge() const
    {
        return _pFontBoldLarge;
    }
}