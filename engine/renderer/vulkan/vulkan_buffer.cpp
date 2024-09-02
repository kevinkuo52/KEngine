#include "vulkan_buffer.h"

VulkanBuffer::VulkanBuffer(VulkanDevice& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) : _device(device)
{
	device.CreateBuffer(size, usage, properties, this->_buffer, this->_bufferMemory);
}

VulkanBuffer::~VulkanBuffer()
{
	vkDestroyBuffer(_device.GetDevice(), this->_buffer, nullptr);
	vkFreeMemory(_device.GetDevice(), this->_bufferMemory, nullptr);
}

void VulkanBuffer::MapMemory(void* src, VkDeviceSize bufferSize)
{
	void* dest;
	vkMapMemory(_device.GetDevice(), _bufferMemory, 0, bufferSize, 0, &dest);
	memcpy(dest, src, (size_t)bufferSize);
	vkUnmapMemory(_device.GetDevice(), _bufferMemory);
}
