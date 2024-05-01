#pragma once

#include <string>
#include "Util.hpp"
#include "WinWindow.h"
#include "ImGuiLogic.h"

namespace ImGuiApp
{
    class Application: NonCopyable
    {
    public:
        bool Init();
        void Loop();
        void Destroy();

    public:
        virtual void InitLanguage();
        virtual Win32Window* CreateMainWindow() = 0;
        virtual ImGuiLogic* CreateMainLogic() = 0;
        virtual std::string GetFileLogPath();

    private:
        Win32Window* pMainWindow;
        ImGuiLogic* pLogic;
    };
}