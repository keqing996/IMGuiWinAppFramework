#include <imgui.h>
#include "ImApp/Component/Button.h"

namespace IMWinApp
{
    Button::Button(const std::string& name, const std::function<void()>& callback)
        : _name(name)
        , _callback(callback)
    {
    }

    void Button::SetText(const std::string& content)
    {
        _name = content;
    }

    void Button::Tick()
    {
        if (ImGui::Button(_name.c_str()))
            _callback();
    }
}
