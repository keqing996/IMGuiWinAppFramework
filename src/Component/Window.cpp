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
        const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(mainViewport->WorkSize, ImGuiCond_Always);

        ImGui::Begin(_name.c_str(), nullptr, _flags);
    }

    void Window::PostTick()
    {
        ImGui::End();
    }

    void Window::SetWindowFlags(ImGuiWindowFlags flags)
    {
        _flags = flags;
    }

    ImGuiWindowFlags Window::GetWindowFlags() const
    {
        return _flags;
    }
}
