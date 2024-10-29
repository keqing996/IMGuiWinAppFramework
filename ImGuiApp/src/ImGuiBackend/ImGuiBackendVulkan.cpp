#include "ImGuiBackendVulkan.h"
#include <backends/imgui_impl_vulkan.h>

namespace ImApp
{
    Backend ImGuiBackendVulkan::GetBackendType() const
    {
        return Backend::Vulkan;
    }

    void ImGuiBackendVulkan::SetupDevice()
    {
    }

    void ImGuiBackendVulkan::SetupImGui()
    {
    }

    void ImGuiBackendVulkan::ClearImGui()
    {
    }

    void ImGuiBackendVulkan::ClearDevice()
    {
    }

    void ImGuiBackendVulkan::NewFrame()
    {
    }

    void ImGuiBackendVulkan::Draw()
    {
    }

    void ImGuiBackendVulkan::SetClearColor(float r, float g, float b, float a)
    {
    }

    void ImGuiBackendVulkan::SwapBuffer()
    {
    }

    void ImGuiBackendVulkan::OnVSyncEnableSettle()
    {
    }
}
