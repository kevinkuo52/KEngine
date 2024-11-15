#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "renderer/vulkan/vulkan_pipeline.h"
#include "renderer/vulkan/vulkan_ray_tracing_pipeline.h"

class Engine {
public:
    
    Engine();
    ~Engine();

    void MainLoop();

private:
    const uint32_t WIDTH = 1200;
    const uint32_t HEIGHT = 1000;
    Window window{ WIDTH, HEIGHT, "VulkanWindow" };
    MainCamera mainCamera{};
    VulkanDevice device{ window, {}};
    TinyObjectImporter importer{};
    Scene scene{ importer, device };
    VulkanPipeline vulkanPipline{ window, mainCamera, device, scene };
	VulkanRayTracingPipeline vulkanRayTracingPipeline{ window };
};