
#include <imgui.h>
#include "ImApp/Component/Text.h"

namespace IMWinApp
{
    Text::Text(const std::string& content)
        : _content(content)
    {
    }

    void Text::SetText(const std::string& content)
    {
        _content = content;
    }

    void Text::Tick()
    {
        ImGui::Text(_content.c_str());
    }
}
