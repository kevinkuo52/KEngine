#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mesh/mesh.h"
#include "renderer/vulkan/vulkan_device.h"
#include "renderer/vulkan/vulkan_pipeline.h"
#include "view/window.h"

class Engine {
public:
    
    Engine();
    ~Engine();

    void MainLoop();

private:

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    Window window{ WIDTH, HEIGHT, "VulkanWindow" };
    VulkanDevice device{ window };
    TinyObjectImporter importer{};
    VulkanPipeline vulkanPipline{ window, device, importer};
};