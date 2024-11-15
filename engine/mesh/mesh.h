#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <array>
#include <memory>
#include <string>
#include "../renderer/vulkan/vulkan_buffer.h"

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct PushConstant {
    alignas(16) glm::mat4 transform;
};

// Push constant structure for the ray tracer
struct PushConstantRay
{
    alignas(16) glm::vec4  clearColor;
    alignas(16) glm::vec3  lightPosition;
    alignas(16) float lightIntensity;
    alignas(16) int   lightType;
};

// The OBJ model
struct ObjModel
{
    uint32_t     nbIndices{ 0 };
    uint32_t     nbVertices{ 0 };
    VulkanBuffer vertexBuffer;    // Device buffer of all 'Vertex'
    VulkanBuffer indexBuffer;     // Device buffer of the indices forming triangles
    VulkanBuffer matColorBuffer;  // Device buffer of array of 'Wavefront material'
    VulkanBuffer matIndexBuffer;  // Device buffer of array of 'Wavefront material'
};

struct ObjInstance
{
    glm::mat4 transform;    // Matrix of the instance
    uint32_t  objIndex{ 0 };  // Model index reference
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription GetBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

class Mesh {
    std::unique_ptr<VulkanBuffer> _vertexBuffer;
    std::unique_ptr<VulkanBuffer> _indexBuffer;
    
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::string texturePath;
    Mesh();
    ~Mesh();
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    void MeshUpload(VulkanDevice& device);
    void Draw(VkCommandBuffer commandBuffer);
private:
    void CreateVertexBuffer(VulkanDevice& device);
    void CreateIndexBuffer(VulkanDevice& device);
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}



