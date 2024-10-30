
#include "ImGuiBackendVulkan.h"
#include "Vulkan/VulkanUtil.h"

namespace ImApp
{
    Backend ImGuiBackendVulkan::GetBackendType() const
    {
        return Backend::Vulkan;
    }

    void ImGuiBackendVulkan::SetupDevice()
    {
        // Glad load vulkan functions.
        int vkVersion = ::gladLoaderLoadVulkan(nullptr, nullptr, nullptr);
        if (vkVersion == 0)
        {
            vulkanAvailable = false;
            return;
        }

        _vkVersionMajor = GLAD_VERSION_MAJOR(vkVersion);
        _vkVersionMinor = GLAD_VERSION_MINOR(vkVersion);

        if (vulkanAvailable)
            VulkanInitInstance();

        ::gladLoaderLoadVulkan(_vkInstance, nullptr, nullptr);

        if (vulkanAvailable)
            VulkanInitDebugReportCallbackExt();

        if (vulkanAvailable)
            VulkanInitSurface();

        if (vulkanAvailable)
            VulkanInitPhysicsDevice();

        ::gladLoaderLoadVulkan(_vkInstance, _vkPhysicalDevice, nullptr);

        if (vulkanAvailable)
            VulkanInitDepthFormat();
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

    void ImGuiBackendVulkan::VulkanInitInstance()
    {
        std::vector<VkLayerProperties> layers;
        if (VulkanUtil::EnumerateInstanceLayerProperties(layers) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        // Validation layers
        std::vector<const char*> validationLayers;
        for (const VkLayerProperties& layer : layers)
        {
            /* Add validation layer.
            if (std::string_view(layer.layerName) == "VK_LAYER_LUNARG_standard_validation")
                validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");
            else if (std::string_view(layer.layerName) == "VK_LAYER_LUNARG_monitor")
                validationLayers.push_back("VK_LAYER_LUNARG_monitor");
                */
        }

        // Platform required extensions
        std::vector<const char*> requiredExtensions {
            // Platform required extensions
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
            // Debug extension
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME
        };

        VkApplicationInfo applicationInfo  = VkApplicationInfo();
        applicationInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName   = "ImGui App";
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pEngineName        = "No Engine";
        applicationInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.apiVersion         = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo    = VkInstanceCreateInfo();
        instanceCreateInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo        = &applicationInfo;
        instanceCreateInfo.enabledLayerCount       = static_cast<std::uint32_t>(validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames     = validationLayers.data();
        instanceCreateInfo.enabledExtensionCount   = static_cast<std::uint32_t>(requiredExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

        if (::vkCreateInstance(&instanceCreateInfo, nullptr, &_vkInstance) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

    }

    void ImGuiBackendVulkan::VulkanInitDebugReportCallbackExt()
    {
        VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo = VkDebugReportCallbackCreateInfoEXT();
        debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debugReportCallbackCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT
            | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
            | VK_DEBUG_REPORT_ERROR_BIT_EXT;
        debugReportCallbackCreateInfo.pfnCallback = VulkanUtil::DebugReportExtCallback;

        // Create the debug callback
        if (::vkCreateDebugReportCallbackEXT(_vkInstance, &debugReportCallbackCreateInfo, nullptr, &_vkDebugReportCallbackExt) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    void ImGuiBackendVulkan::VulkanInitSurface()
    {
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = VkWin32SurfaceCreateInfoKHR();
        surfaceCreateInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.hinstance = GetModuleHandleW(nullptr);
        surfaceCreateInfo.hwnd      = static_cast<HWND>(_pWindow->GetSystemHandle());

        if (::vkCreateWin32SurfaceKHR(_vkInstance, &surfaceCreateInfo, nullptr, &_vkSurface) == VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }
    }

    void ImGuiBackendVulkan::VulkanInitPhysicsDevice()
    {
        std::vector<VkPhysicalDevice> devices;
        if (VulkanUtil::EnumeratePhysicalDevices(_vkInstance, devices) != VK_SUCCESS)
        {
            vulkanAvailable = false;
            return;
        }

        VkPhysicalDeviceType lastFoundDeviceType = VK_PHYSICAL_DEVICE_TYPE_OTHER;
        for (VkPhysicalDevice singleDevice : devices)
        {
            if (!VulkanUtil::IsPhysicalDeviceSupportSwapChain(singleDevice))
                continue;

            VkPhysicalDeviceProperties deviceProperties;
            ::vkGetPhysicalDeviceProperties(singleDevice, &deviceProperties);

            // Discrete gpu found, no need to continue.
            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                _vkPhysicalDevice = singleDevice;
                break;
            }

            // Integrated gpu found, record it and keep finding discrete gpu.
            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            {
                _vkPhysicalDevice = singleDevice;
                lastFoundDeviceType = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
            }

            // Cpu found, record it only if that no gpu was found.
            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU
                && lastFoundDeviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            {
                _vkPhysicalDevice = singleDevice;
                lastFoundDeviceType = VK_PHYSICAL_DEVICE_TYPE_CPU;
            }
        }

        if (!_vkPhysicalDevice)
        {
            vulkanAvailable = false;
            return;
        }
    }

    void ImGuiBackendVulkan::VulkanInitDepthFormat()
    {
        VkFormatProperties formatProperties = VkFormatProperties();

        ::vkGetPhysicalDeviceFormatProperties(_vkPhysicalDevice, VK_FORMAT_D24_UNORM_S8_UINT, &formatProperties);

        if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            _vkDepthFormat = VK_FORMAT_D24_UNORM_S8_UINT;
        }
        else
        {
            ::vkGetPhysicalDeviceFormatProperties(_vkPhysicalDevice, VK_FORMAT_D32_SFLOAT_S8_UINT, &formatProperties);

            if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            {
                _vkDepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
            }
            else
            {
                ::vkGetPhysicalDeviceFormatProperties(_vkPhysicalDevice, VK_FORMAT_D32_SFLOAT, &formatProperties);

                if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
                {
                    _vkDepthFormat = VK_FORMAT_D32_SFLOAT;
                }
                else
                {
                    vulkanAvailable = false;
                    return;
                }
            }
        }
    }
}
