#include "vulkan_ray_tracing_pipeline.h"
#include <array>
#include "../shader/ray_trace/shader.h"
#include "../../mesh/mesh.h"

VulkanRayTracingPipeline::VulkanRayTracingPipeline(Window& window) : window{ window } {

}

VulkanRayTracingPipeline::~VulkanRayTracingPipeline() {

}

void VulkanRayTracingPipeline::RayTrace(const VkCommandBuffer& cmdBuf, const glm::vec4& clearColor)
{
	// Initializing push constant values
	pcRay.clearColor = clearColor;
	pcRay.lightPosition = glm::vec3(2.0f, 2.0f, 1.0f);
	pcRay.lightIntensity = 300; // 0 - 500
	pcRay.lightType = 0; // point light

	std::vector<VkDescriptorSet> descSets{ m_rtDescSet, m_descSet };
	vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, m_rtPipeline);
	vkCmdBindDescriptorSets(cmdBuf, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, m_rtPipelineLayout, 0,
		(uint32_t)descSets.size(), descSets.data(), 0, nullptr);
	vkCmdPushConstants(cmdBuf, m_rtPipelineLayout,
		VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_MISS_BIT_KHR,
		0, sizeof(PushConstantRay), &pcRay);


	vkCmdTraceRaysKHR(cmdBuf, &m_rgenRegion, &m_missRegion, &m_hitRegion, &m_callRegion, m_size.width, m_size.height, 1);
}

void VulkanRayTracingPipeline::CreateRayTracingPipeline()
{
	// All stages
	std::array<VkPipelineShaderStageCreateInfo, eShaderGroupCount> stages{};
	VkPipelineShaderStageCreateInfo stage{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
	stage.pName = "main";  // All the same entry point
	// Raygen
	stage.module = shader::CreateShaderModule(device.GetDevice(), shader::ReadFile("engine/renderer/shader/spv/raytrace.rgen.spv"));
	stage.stage = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
	stages[eRaygen] = stage;
	// Miss
	stage.module = shader::CreateShaderModule(device.GetDevice(), shader::ReadFile("engine/renderer/shader/spv/raytrace.rmiss.spv"));
	stage.stage = VK_SHADER_STAGE_MISS_BIT_KHR;
	stages[eMiss] = stage;
	// The second miss shader is invoked when a shadow ray misses the geometry. It simply indicates that no occlusion has been found
	stage.module =
		shader::CreateShaderModule(device.GetDevice(), shader::ReadFile("engine/renderer/shader/spv/raytrace_shadow.rmiss.spv"));
	stage.stage = VK_SHADER_STAGE_MISS_BIT_KHR;
	stages[eMiss2] = stage;
	// Hit Group - Closest Hit
	stage.module = shader::CreateShaderModule(device.GetDevice(), shader::ReadFile("engine/renderer/shader/spv/raytrace.rchit.spv"));
	stage.stage = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
	stages[eClosestHit] = stage;

	// Shader groups
	VkRayTracingShaderGroupCreateInfoKHR group{ VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR };
	group.anyHitShader = VK_SHADER_UNUSED_KHR;
	group.closestHitShader = VK_SHADER_UNUSED_KHR;
	group.generalShader = VK_SHADER_UNUSED_KHR;
	group.intersectionShader = VK_SHADER_UNUSED_KHR;
	
	// Raygen
	group.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
	group.generalShader = eRaygen;
	rtShaderGroups.push_back(group);

	// Miss
	group.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
	group.generalShader = eMiss;
	rtShaderGroups.push_back(group);

	// Shadow Miss
	group.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
	group.generalShader = eMiss2;
	rtShaderGroups.push_back(group);

	// closest hit shader
	group.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
	group.generalShader = VK_SHADER_UNUSED_KHR;
	group.closestHitShader = eClosestHit;
	rtShaderGroups.push_back(group);

	// Push constant: we want to be able to update constants used by the shaders
	VkPushConstantRange pushConstant{ VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_MISS_BIT_KHR,
									 0, sizeof(PushConstantRay) };
	
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstant;

	vkCreatePipelineLayout(device.GetDevice(), &pipelineLayoutCreateInfo, nullptr, &rtPipelineLayout);

	// Assemble the shader stages and recursion depth info into the ray tracing pipeline
	VkRayTracingPipelineCreateInfoKHR rayPipelineInfo{ VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR };
	rayPipelineInfo.stageCount = static_cast<uint32_t>(stages.size());  // Stages are shaders
	rayPipelineInfo.pStages = stages.data();

	// In this case, m_rtShaderGroups.size() == 4: we have one raygen group,
	// two miss shader groups, and one hit group.
	rayPipelineInfo.groupCount = static_cast<uint32_t>(rtShaderGroups.size());
	rayPipelineInfo.pGroups = rtShaderGroups.data();

	// The ray tracing process can shoot rays from the camera, and a shadow ray can be shot from the
	// hit points of the camera rays, hence a recursion level of 2. This number should be kept as low
	// as possible for performance reasons. Even recursive ray tracing should be flattened into a loop
	// in the ray generation to avoid deep recursion.
	rayPipelineInfo.maxPipelineRayRecursionDepth = 2;  // Ray depth
	rayPipelineInfo.layout = rtPipelineLayout;

	vkCreateRayTracingPipelinesKHR(device.GetDevice(), {}, {}, 1, &rayPipelineInfo, nullptr, &rtPipeline);

	// Spec only guarantees 1 level of "recursion". Check for that sad possibility here.
	if (m_rtProperties.maxRayRecursionDepth <= 1)
	{
		throw std::runtime_error("Device fails to support ray recursion (m_rtProperties.maxRayRecursionDepth <= 1)");
	}

	for (auto& s : stages) {
		vkDestroyShaderModule(device.GetDevice(), s.module, nullptr);
	}
}

void VulkanRayTracingPipeline::CreateShaderBindingTable()
{
	uint32_t missCount{ 2 };
	uint32_t hitCount{ 1 };
	auto     handleCount = 1 + missCount + hitCount;
	uint32_t handleSize = m_rtProperties.shaderGroupHandleSize;

	// The SBT (buffer) need to have starting groups to be aligned and handles in the group to be aligned.
	uint32_t handleSizeAligned = nvh::align_up(handleSize, m_rtProperties.shaderGroupHandleAlignment);

	m_rgenRegion.stride = nvh::align_up(handleSizeAligned, m_rtProperties.shaderGroupBaseAlignment);
	m_rgenRegion.size = m_rgenRegion.stride;  // The size member of pRayGenShaderBindingTable must be equal to its stride member
	m_missRegion.stride = handleSizeAligned;
	m_missRegion.size = nvh::align_up(missCount * handleSizeAligned, m_rtProperties.shaderGroupBaseAlignment);
	m_hitRegion.stride = handleSizeAligned;
	m_hitRegion.size = nvh::align_up(hitCount * handleSizeAligned, m_rtProperties.shaderGroupBaseAlignment);

	// Get the shader group handles
	uint32_t             dataSize = handleCount * handleSize;
	std::vector<uint8_t> handles(dataSize);
	auto result = vkGetRayTracingShaderGroupHandlesKHR(m_device, m_rtPipeline, 0, handleCount, dataSize, handles.data());
	assert(result == VK_SUCCESS);

	// Allocate a buffer for storing the SBT.
	VkDeviceSize sbtSize = m_rgenRegion.size + m_missRegion.size + m_hitRegion.size + m_callRegion.size;
	m_rtSBTBuffer = m_alloc.createBuffer(sbtSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		| VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	m_debug.setObjectName(m_rtSBTBuffer.buffer, std::string("SBT"));  // Give it a debug name for NSight.

	// Find the SBT addresses of each group
	VkBufferDeviceAddressInfo info{ VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO, nullptr, m_rtSBTBuffer.buffer };
	VkDeviceAddress           sbtAddress = vkGetBufferDeviceAddress(m_device, &info);
	m_rgenRegion.deviceAddress = sbtAddress;
	m_missRegion.deviceAddress = sbtAddress + m_rgenRegion.size;
	m_hitRegion.deviceAddress = sbtAddress + m_rgenRegion.size + m_missRegion.size;

	// Helper to retrieve the handle data
	auto getHandle = [&](int i) { return handles.data() + i * handleSize; };

	// Map the SBT buffer and write in the handles.
	auto* pSBTBuffer = reinterpret_cast<uint8_t*>(m_alloc.map(m_rtSBTBuffer));
	uint8_t* pData{ nullptr };
	uint32_t handleIdx{ 0 };
	// Raygen
	pData = pSBTBuffer;
	memcpy(pData, getHandle(handleIdx++), handleSize);
	// Miss
	pData = pSBTBuffer + m_rgenRegion.size;
	for (uint32_t c = 0; c < missCount; c++)
	{
		memcpy(pData, getHandle(handleIdx++), handleSize);
		pData += m_missRegion.stride;
	}
	// Hit
	pData = pSBTBuffer + m_rgenRegion.size + m_missRegion.size;
	for (uint32_t c = 0; c < hitCount; c++)
	{
		memcpy(pData, getHandle(handleIdx++), handleSize);
		pData += m_hitRegion.stride;
	}

	m_alloc.unmap(m_rtSBTBuffer);
	m_alloc.finalizeAndReleaseStaging();
}

