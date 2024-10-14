#pragma once

#include <array>
#include "ImApp/ImGuiBackend/ImGuiBackend.h"

namespace IMWinApp
{
    class ImGuiBackendVulkan: public ImGuiBackend
    {
    public:
        void SetupDevice() override;
        void SetupImGui() override;
        void ClearImGui() override;
        void ClearDevice() override;
        void NewFrame() override;
        void Draw() override;
        void SetClearColor(const Utility::Color& color) override;
        void SwapBuffer() override;

    private:
        std::array<float, 4> _clearColor = { 0.75f, 0.75f, 0.75f, 1.00f };
    };
}
