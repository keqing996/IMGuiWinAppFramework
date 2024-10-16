#pragma once

#include <initializer_list>
#include <functional>
#include <imgui.h>

namespace ImApp
{
    struct SameLine
    {
        SameLine(std::initializer_list<std::function<void()>> contents);
    };
}