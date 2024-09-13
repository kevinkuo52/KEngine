#pragma once
#include "../mesh/mesh.h"
class SceneNode
{
	const std::string name;
	glm::mat4 transformation;
	std::shared_ptr<Mesh> mesh;
	SceneNode* parent;
	std::vector<SceneNode*> children;

public:
	SceneNode(std::string name, std::shared_ptr<Mesh> mesh = nullptr, SceneNode* parent = nullptr, std::vector<SceneNode*> children = {});
	
	std::string GetName() { return name; };
	glm::mat4 GetTransformation() { return transformation; };
	void SetTransformation(glm::mat4 newTransform) { transformation = newTransform; };
	std::shared_ptr<Mesh> GetMesh() { return mesh; };
	SceneNode* GetParent() { return parent; };
	std::vector<SceneNode*> GetChildren() { return children; };
	void Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);
};

