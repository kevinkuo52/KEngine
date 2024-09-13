#include "scene_node.h"

SceneNode::SceneNode(std::string name, std::shared_ptr<Mesh> mesh, SceneNode* parent, std::vector<SceneNode*> children)
	: name(name), mesh(mesh), parent(parent), children(children)
{
	transformation = glm::mat4(1.0f);
}

void SceneNode::Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout)
{
	vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &transformation);
	mesh->Draw(commandBuffer);
}
