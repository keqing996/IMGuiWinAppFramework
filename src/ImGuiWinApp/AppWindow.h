#pragma once

namespace ImApp
{
    enum class ImGuiBackendType
    {
        DX11,
    };

    class AppWindow
    {
    public:
        explicit AppWindow(ImGuiBackendType backend = ImGuiBackendType::DX11);

    private:

    };
}