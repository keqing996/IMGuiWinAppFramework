#pragma once

#include <initializer_list>
#include <functional>
#include <imgui.h>

namespace IMWinApp
{
    struct SameLine
    {
        SameLine(std::initializer_list<std::function<void()>> contents)
        {
            int current = 0;
            for (auto& ptr: contents)
            {
                ptr();
                if (current < contents.size() - 1)
                    ImGui::SameLine();
                current++;
            }
        }
    };
}