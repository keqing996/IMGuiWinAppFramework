#include "ImApp/Component/Window.h"

namespace ImApp
{
    ImWindow::ImWindow(const char* name, ImGuiWindowFlags flags, const std::function<void()>& impl)
    {
        ImGui::Begin(name, nullptr, flags);

        if (impl != nullptr)
            impl();
    }

    ImWindow::~ImWindow()
    {
        ImGui::End();
    }
}
