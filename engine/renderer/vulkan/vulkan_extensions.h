#pragma once
#include <vulkan/vulkan.h>

class vulkan_extensions
{
};

/* Load all avaiable extensions */
void LoadVulkanExtensions(VkInstance instance, PFN_vkGetInstanceProcAddr getInstanceProcAddr, VkDevice device, PFN_vkGetDeviceProcAddr getDeviceProcAddr);