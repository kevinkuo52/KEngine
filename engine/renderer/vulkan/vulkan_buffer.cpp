#include "vulkan_buffer.h"

VulkanBuffer::VulkanBuffer(VulkanDevice& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) : _device(device)
{
	this->_device.CreateBuffer(size, usage, properties, this->_buffer, this->_bufferMemory);
}

VulkanBuffer::~VulkanBuffer()
{
	vkDestroyBuffer(this->_device.GetDevice(), this->_buffer, nullptr);
	vkFreeMemory(this->_device.GetDevice(), this->_bufferMemory, nullptr);
}

void VulkanBuffer::MapMemory(void* src, VkDeviceSize bufferSize)
{
	void* dest;
	vkMapMemory(this->_device.GetDevice(), this->_bufferMemory, 0, bufferSize, 0, &dest);
	memcpy(dest, src, (size_t)bufferSize);
	vkUnmapMemory(this->_device.GetDevice(), this->_bufferMemory);
}
