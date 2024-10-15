#pragma once

#include <string>
#include <memory>
#include <functional>
#include "ImGuiBackend/ImGuiBackend.h"
#include "Utility/NonCopyable.h"
#include "NativeWindow/Window.h"

class ImFont;
class ImFontAtlas;

namespace IMWinApp
{
    class ImGuiWinApp : Utility::NonCopyable
    {
    public:
        ImGuiWinApp() = default;
        ~ImGuiWinApp();

    public:
        void InitWindow(int width, int height, const std::string& title, int style = NativeWindow::DefaultWindowStyle, Backend backend = Backend::D3d11);
        void Loop();
        void CloseWindow();
        NativeWindow::Window* GetNativeWindow();

        // Option
        void SetOnEventHandler(const std::function<bool(const NativeWindow::WindowEvent&, bool*)>& handler);
        void SetVSyncEnable(bool enable);
        bool GetVSyncEnable() const;
        void SetClearColor(const Utility::Color& color);

        // Font
        int GetNormalFontSize();
        int GetLargeFontSize();
        ImFont* CreateImGuiFont(void* fontData, int fontDataSize, int fontSize, bool transferDataOwnership = true);
        ImFont* CreateImGuiFont(const std::string& ttfPath, int fontSize);
        ImFont* GetFontRegularNormal() const;
        ImFont* GetFontRegularLarge() const;
        ImFont* GetFontBoldNormal() const;
        ImFont* GetFontBoldLarge() const;

    protected:
        virtual void PreTick();
        virtual void Tick();
        virtual void PostTick();

    private:
        void ImGuiInitConfig();
        void ImGuiInitFrontend();
        void ImGuiInitBackend();
        void ImGuiInitFont();
        void InitLocale();

    private:
        // Window
        std::unique_ptr<NativeWindow::Window> _pWindow = nullptr;
        std::function<bool(const NativeWindow::WindowEvent&, bool*)> _onEventHandler = nullptr;

        // Backend
        std::unique_ptr<ImGuiBackend> _pBackend = nullptr;

        // Font
        static constexpr int NORMAL_FONT_SIZE = 16;
        static constexpr int LARGE_FONT_SIZE = 20;
        std::shared_ptr<ImFontAtlas> _pSharedImGuiFonts = nullptr;
        ImFont* _pFontRegularNormal = nullptr;
        ImFont* _pFontRegularLarge = nullptr;
        ImFont* _pFontBoldNormal = nullptr;
        ImFont* _pFontBoldLarge = nullptr;
    };
}