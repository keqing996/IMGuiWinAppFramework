#pragma once

#include "ImGui/imgui.h"
#include <Helper/NonCopyable.h>

namespace ImGuiApp
{
    class Win32Window;

    class ImGuiLogic : public Helper::NonCopyable
    {
    public:
        ImGuiLogic();
        virtual ~ImGuiLogic();

    public:
        void SetUp(Win32Window* pWindow);
        void Loop();
        void Destroy();

    protected:
        virtual void Update() = 0;

    protected:
        // Top Window
        Win32Window* _pTopWindow;
    };
}