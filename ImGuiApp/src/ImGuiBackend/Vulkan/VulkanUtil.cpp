
#include "VulkanUtil.h"

namespace ImApp
{
    VkResult VulkanUtil::EnumerateInstanceLayerProperties(std::vector<VkLayerProperties>& layers)
    {
        uint32_t layersCount = 0;

        VkResult result = ::vkEnumerateInstanceLayerProperties(&layersCount, nullptr);
        if (result != VK_SUCCESS)
            return result;

        layers.resize(layersCount);

        return ::vkEnumerateInstanceLayerProperties(&layersCount, layers.data());
    }

    VkResult VulkanUtil::EnumeratePhysicalDevices(VkInstance instance, std::vector<VkPhysicalDevice>& devices)
    {
        std::uint32_t deviceCount = 0;

        VkResult result = ::vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (result != VK_SUCCESS)
            return result;

        devices.resize(deviceCount);

        return ::vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    }

    VkResult VulkanUtil::EnumerateDeviceExtensionProperties(VkPhysicalDevice device, std::vector<VkExtensionProperties>& extensions)
    {
        std::uint32_t extensionCount = 0;

        VkResult result = ::vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        if (result != VK_SUCCESS)
            return result;

        extensions.resize(extensionCount);

        return ::vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());
    }

    bool VulkanUtil::IsPhysicalDeviceSupportSwapChain(VkPhysicalDevice device)
    {
        std::vector<VkExtensionProperties> extensions;
        if (EnumerateDeviceExtensionProperties(device, extensions) != VK_SUCCESS)
            return false;

        for (VkExtensionProperties& extension : extensions)
        {
            if (std::string_view(extension.extensionName) == VK_KHR_SWAPCHAIN_EXTENSION_NAME)
                return true;
        }

        return false;
    }

    VkBool32 VulkanUtil::DebugReportExtCallback(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, std::uint64_t,
                                                std::size_t, std::int32_t, const char*, const char* pMessage, void*)
    {
        std::cerr << pMessage << std::endl;

        return VK_FALSE;
    }
}
