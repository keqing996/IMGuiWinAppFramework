#pragma once

#include <iostream>
#include <vector>
#include "VulkanInclude.h"

namespace ImApp
{
    class VulkanUtil
    {
    public:
        VulkanUtil() = delete;

    public:
        static VkResult
        EnumerateInstanceLayerProperties(std::vector<VkLayerProperties>& layers);

        static VkResult
        EnumeratePhysicalDevices(VkInstance instance, std::vector<VkPhysicalDevice>& devices);

        static VkResult
        EnumerateDeviceExtensionProperties(VkPhysicalDevice device, std::vector<VkExtensionProperties>& extensions);

        static bool
        IsPhysicalDeviceSupportSwapChain(VkPhysicalDevice device);

        static VKAPI_ATTR VkBool32 VKAPI_CALL
        DebugReportExtCallback(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, std::uint64_t, std::size_t, std::int32_t, const char*, const char* pMessage, void*);
    };
}