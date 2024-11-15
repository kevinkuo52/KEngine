#pragma once
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <optional>
#include <vector>
#include "../../view/window.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanDevice
{
public:
    const unsigned int VULKAN_API_VERSION_MAJOR = 1;
	const unsigned int VULKAN_API_VERSION_MINOR = 3;
    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    VulkanDevice(Window& window, const std::vector<void*> properties2);
    ~VulkanDevice();

    VkCommandPool GetCommandPool() { return commandPool; }
    VkDevice GetDevice() { return device; }
    VkSurfaceKHR GetSurface() { return surface; }
    VkQueue GetGraphicsQueue() { return graphicsQueue; }
    VkQueue GetPresentQueue() { return presentQueue; }
    SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(physicalDevice); }
    QueueFamilyIndices GetPhysicalQueueFamilies() { return FindQueueFamilies(physicalDevice); }
    VkPhysicalDeviceProperties GetPhysicalDeviceProperties() { return properties; };
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkFormat FindSupportedFormat(
        const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    // Buffer Helper Functions
    void CreateBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer& buffer,
        VkDeviceMemory& bufferMemory);
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void CopyBufferToImage(
        VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

private:
    void CreateInstance();
    void SetupDebugMessenger();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateCommandPool();
    void CreatePhysicalDeviceProperties2();

    // helper functions
    bool IsDeviceSuitable(VkPhysicalDevice device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    std::vector<const char*> GetRequiredExtensions();
    bool CheckValidationLayerSupport();
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);


    VkInstance instance;
    Window& window;
    const std::vector<void*> properties2;
    VkSurfaceKHR surface;
    // implicitly destroyed when the VkInstance is destroyed
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    // implicitly destroyed when the device is destroyed
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkCommandPool commandPool;


    VkPhysicalDeviceProperties properties{};

    VkDebugUtilsMessengerEXT debugMessenger;
   
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> deviceExtensions = { 
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		// ray tracing extensions
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
            // dependency of ACCELERATION_STRUCTURE
            //VK_VERSION_1_1,
            //VK_EXT_descriptor_indexing, 
            //VK_KHR_buffer_device_address,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        
        };
    VkPhysicalDeviceAccelerationStructureFeaturesKHR accelFeature{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR };
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR rtPipelineFeature{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR };

    const std::vector<void*> featureStructs = {
        &accelFeature,
        &rtPipelineFeature
    };

    struct ExtensionHeader  // Helper struct to link extensions together
    {
        VkStructureType sType;
        void* pNext;
    };

    // This struct holds all core feature information for a physical device
    struct PhysicalDeviceInfo
    {
        VkPhysicalDeviceMemoryProperties     memoryProperties{};
        std::vector<VkQueueFamilyProperties> queueProperties;

        VkPhysicalDeviceFeatures         features10{};
        VkPhysicalDeviceVulkan11Features features11{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES };
        VkPhysicalDeviceVulkan12Features features12{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
        VkPhysicalDeviceVulkan13Features features13{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };

        VkPhysicalDeviceProperties         properties10{};
        VkPhysicalDeviceVulkan11Properties properties11{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES };
        VkPhysicalDeviceVulkan12Properties properties12{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES };
        VkPhysicalDeviceVulkan13Properties properties13{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES };
    };

    PhysicalDeviceInfo m_physicalInfo;
};

