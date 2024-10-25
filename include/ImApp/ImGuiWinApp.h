#pragma once

#include <string>
#include <memory>
#include "ImGuiBackend/ImGuiBackend.h"
#include "NativeWindow/Window.h"

class ImFont;
class ImFontAtlas;

namespace ImApp
{
    class ImGuiWinApp : NativeWindow::Window
    {
    public:
        explicit ImGuiWinApp(Backend backend = Backend::D3d11);
        ~ImGuiWinApp() override = default;

    public:
        void Loop();
        void CloseWindow();

        // Option
        void SetVSyncEnable(bool enable);
        bool GetVSyncEnable() const;
        void SetClearColor(const Utility::Color& color);

    protected:
        void OnWindowCreated() override;
        bool WindowEventPreProcess(uint32_t message, void* wpara, void* lpara, int* result) override;
        void OnWindowClose() override;
        void OnWindowPreDestroy() override;
        void OnWindowPostDestroy() override;
        void OnWindowResize(int width, int height) override;
        void OnWindowGetFocus() override;
        void OnWindowLostFocus() override;
        void OnMouseEnterWindow() override;
        void OnMouseLeaveWindow() override;

        virtual void PreTick();
        virtual void Tick();
        virtual void PostTick();

        // Dpi
        float GetDpiScale();

        // Font
        int GetDefaultFontSize();
        ImFont* CreateImGuiFont(void* fontData, int fontDataSize, int fontSize, bool transferDataOwnership = true);
        ImFont* CreateImGuiFont(const std::string& ttfPath, int fontSize);

    private:
        void ImGuiInitConfig();
        void ImGuiInitFrontend();
        void ImGuiInitBackend();
        void InitTheme();

    private:
        std::unique_ptr<ImGuiBackend> _pBackend = nullptr;
    };
}