#include "ImApp/Component/SameLine.h"

namespace ImApp
{
    SameLine::SameLine(std::initializer_list<std::function<void()>> contents)
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
}
