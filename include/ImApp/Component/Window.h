#pragma once

#include <functional>
#include <imgui.h>

namespace IMWinApp
{
    struct Window
    {
        Window(const char* name, ImGuiWindowFlags flags, const std::function<void()>& impl);

        ~Window();
    };
}