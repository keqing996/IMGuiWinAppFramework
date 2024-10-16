#include "ImApp/Component/Window.h"

namespace ImApp
{
    Window::Window(const char* name, ImGuiWindowFlags flags, const std::function<void()>& impl)
    {
        ImGui::Begin(name, nullptr, flags);

        if (impl != nullptr)
            impl();
    }

    Window::~Window()
    {
        ImGui::End();
    }
}
