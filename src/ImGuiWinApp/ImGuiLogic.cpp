#include "ImGuiLogic.h"
#include "ImGuiWinApp/WinWindow.h"

namespace ImGuiApp
{
    ImGuiLogic::ImGuiLogic(): _pTopWindow(nullptr)
    {
    }

    ImGuiLogic::~ImGuiLogic()
    {
        Destroy();
    }

    void ImGuiLogic::SetUp(Win32Window* pWindow)
    {
        _pTopWindow = pWindow;
        _pTopWindow->AddImGuiLogicUpdater(this);
    }

    void ImGuiLogic::Loop()
    {
        if (_pTopWindow == nullptr)
            return;

        Update();
    }

    void ImGuiLogic::Destroy()
    {
        if (_pTopWindow != nullptr)
        {
            _pTopWindow->RemoveImGuiLogicUpdater(this);
            _pTopWindow = nullptr;
        }
    }
}