#include "mesh.h"


// Describes the stride between vertices
VkVertexInputBindingDescription Vertex::GetBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

// describe how to read each Vertext struct and bind it to input of vertext shader
std::array<VkVertexInputAttributeDescription, 3> Vertex::GetAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::MeshUpload(VulkanDevice& device)
{
    CreateVertexBuffer(device);
    CreateIndexBuffer(device);
}

void Mesh::Draw(VkCommandBuffer commandBuffer)
{
    VkBuffer vertexBuffers[] = { _vertexBuffer->GetVkBuffer()};
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, _indexBuffer->GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Mesh::CreateVertexBuffer(VulkanDevice& device)
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VulkanBuffer stagingBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    

    stagingBuffer.MapMemory(vertices.data(), bufferSize);

    _vertexBuffer = std::make_unique<VulkanBuffer>(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	device.CopyBuffer(stagingBuffer.GetVkBuffer(), _vertexBuffer->GetVkBuffer(), bufferSize);
}

void Mesh::CreateIndexBuffer(VulkanDevice& device)
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
	VulkanBuffer stagingBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    stagingBuffer.MapMemory(vertices.data(), bufferSize);

    _indexBuffer = std::make_unique<VulkanBuffer>(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    device.CopyBuffer(stagingBuffer.GetVkBuffer(), _indexBuffer->GetVkBuffer(), bufferSize);
}
