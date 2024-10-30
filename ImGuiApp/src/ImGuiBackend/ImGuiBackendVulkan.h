#pragma once

#include <array>
#include <backends/imgui_impl_vulkan.h>
#include "Vulkan/VulkanInclude.h"
#include "ImApp/ImGuiBackend/ImGuiBackend.h"

namespace ImApp
{
    class ImGuiBackendVulkan: public ImGuiBackend
    {
    public:
        Backend GetBackendType() const override;
        void SetupDevice() override;
        void SetupImGui() override;
        void ClearImGui() override;
        void ClearDevice() override;
        void NewFrame() override;
        void Draw() override;
        void SetClearColor(float r, float g, float b, float a) override;
        void SwapBuffer() override;

    protected:
        void OnVSyncEnableSettle() override;

    private:
        void VulkanInitInstance();
        void VulkanInitDebugReportCallbackExt();
        void VulkanInitSurface();
        void VulkanInitPhysicsDevice();
        void VulkanInitDepthFormat();

    private:
        std::array<float, 4> _clearColor = { 0.75f, 0.75f, 0.75f, 1.00f };

        bool vulkanAvailable = true;
        int _vkVersionMajor = 0;
        int _vkVersionMinor = 0;

        VkInstance               _vkInstance = nullptr;
        VkDebugReportCallbackEXT        _vkDebugReportCallbackExt = nullptr;
        VkSurfaceKHR _vkSurface = nullptr;
        VkPhysicalDevice         _vkPhysicalDevice = nullptr;
        VkFormat                        _vkDepthFormat = VK_FORMAT_UNDEFINED;



        VkAllocationCallbacks*   g_Allocator = nullptr;
        VkDevice                 g_Device = VK_NULL_HANDLE;
        uint32_t                 g_QueueFamily = (uint32_t)-1;
        VkQueue                  g_Queue = VK_NULL_HANDLE;
        VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
        VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

        ImGui_ImplVulkanH_Window g_MainWindowData;
        int                      g_MinImageCount = 2;
        bool                     g_SwapChainRebuild = false;
    };
}
