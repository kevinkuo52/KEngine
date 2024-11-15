#pragma once
#include "vulkan_device.h"

class VulkanRayTracingPipeline
{
public:
	VulkanRayTracingPipeline(Window& window);
	~VulkanRayTracingPipeline();

	void RayTrace(const VkCommandBuffer& cmdBuf, const glm::vec4& clearColor);
private:
	Window& window;

	// #VKRay
	VkPhysicalDeviceRayTracingPipelinePropertiesKHR m_rtProperties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR };
	const std::vector<void*> properties2 = { &m_rtProperties };
	VulkanDevice device{ window,  properties2 };

	std::vector<VkRayTracingShaderGroupCreateInfoKHR> rtShaderGroups;
	VkPipelineLayout rtPipelineLayout;
	VkPipeline rtPipeline;

	PushConstantRay pcRay{};

	enum StageIndices
	{
		eRaygen,
		eMiss,
		eMiss2,
		eClosestHit,
		eShaderGroupCount
	};

	void CreateRayTracingPipeline();
	void CreateShaderBindingTable();
};