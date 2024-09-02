#pragma once
#include "vulkan_device.h"

class VulkanBuffer
{
    VkBuffer _buffer = VK_NULL_HANDLE;
    VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;
    VulkanDevice& _device;

public:
    VulkanBuffer(VulkanDevice& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    ~VulkanBuffer();
    VkBuffer GetVkBuffer() { return _buffer; };
    //VkDeviceMemory GetVkDeviceMemory() { return _bufferMemory; };
    void MapMemory(void* src, VkDeviceSize bufferSize);
};
