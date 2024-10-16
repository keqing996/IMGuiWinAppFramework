#pragma once

#include <functional>
#include <imgui.h>

namespace ImApp
{
    struct Window
    {
        Window(const char* name, ImGuiWindowFlags flags, const std::function<void()>& impl);

        ~Window();
    };
}