#pragma once

#include <string>
#include <memory>
#include <functional>
#include <d3d11.h>
#include "Utility/NonCopyable.h"
#include "NativeWindow/Window.h"

class ImFont;
class ImFontAtlas;

namespace IMWinApp
{
    class ImGuiWinApp final : Utility::NonCopyable
    {
    public:
        enum class TickStage
        {
            PreFrame,
            OnFrame,
            PostFrame,
        };

    public:
        ImGuiWinApp(int width, int height, const std::string& title, int style = NativeWindow::WindowStyleDefault);
        ~ImGuiWinApp();

    public:
        void SetOnEventHandler(const std::function<bool(const NativeWindow::WindowEvent&, bool*)>& handler);
        void AppLoop();
        void EnableVSync(bool enable);
        void CloseWindow();
        NativeWindow::Window& GetNativeWindow();

        // Font
        int GetNormalFontSize();
        int GetLargeFontSize();
        ImFont* CreateImGuiFont(void* fontData, int fontDataSize, int fontSize, bool transferDataOwnership = true);
        ImFont* CreateImGuiFont(const std::string& ttfPath, int fontSize);
        ImFont* GetFontRegularNormal() const;
        ImFont* GetFontRegularLarge() const;
        ImFont* GetFontBoldNormal() const;
        ImFont* GetFontBoldLarge() const;

    public:
        template<TickStage stage>
        void SetTickFunction(const std::function<void(ImGuiWinApp&)>& tickFunction)
        {
            if constexpr (stage == TickStage::PreFrame)
                _preFrameTick = tickFunction;
            else if constexpr (stage == TickStage::OnFrame)
                _onFrameTick = tickFunction;
            else if constexpr (stage == TickStage::PostFrame)
                _postFrameTick = tickFunction;
        }

    private:
        void DefaultOnEventHandler(const NativeWindow::WindowEvent& e, bool* breakAppLoop);

    private:
        // Window
        NativeWindow::Window _window;
        std::function<bool(const NativeWindow::WindowEvent&, bool*)> _onEventHandler = nullptr;

        // Option
        bool _enableVSync = true;

        // Updater
        std::function<void(ImGuiWinApp&)> _preFrameTick = nullptr;
        std::function<void(ImGuiWinApp&)> _onFrameTick = nullptr;
        std::function<void(ImGuiWinApp&)> _postFrameTick = nullptr;

        // Font
        static constexpr int NORMAL_FONT_SIZE = 16;
        static constexpr int LARGE_FONT_SIZE = 20;
        std::unique_ptr<ImFontAtlas> _pSharedImGuiFonts = nullptr;
        ImFont* _pFontRegularNormal = nullptr;
        ImFont* _pFontRegularLarge = nullptr;
        ImFont* _pFontBoldNormal = nullptr;
        ImFont* _pFontBoldLarge = nullptr;
    };
}