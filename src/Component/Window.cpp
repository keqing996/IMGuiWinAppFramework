#include "ImApp/Component/Window.h"

namespace IMWinApp
{
    Window::Window(const std::string& name, ImGuiWindowFlags flags)
        : _name(name)
        , _flags(flags)
    {
    }

    void Window::PreTick()
    {
        ImGui::Begin(_name.c_str(), nullptr, _flags);
    }

    void Window::PostTick()
    {
        ImGui::End();
    }
}
