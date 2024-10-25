#pragma once

#include <functional>
#include <imgui.h>

namespace ImApp
{
    struct ImWindow
    {
        ImWindow(const char* name, ImGuiWindowFlags flags, const std::function<void()>& impl);

        ~ImWindow();
    };
}