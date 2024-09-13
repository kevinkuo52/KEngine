#pragma once
#include "../mesh/mesh.h"
#include "../importer/tinyobj_importer.h"
#include "scene_node.h"

class Scene
{
	TinyObjectImporter& _importer;
	VulkanDevice& _device;
	std::unordered_map<std::string, SceneNode*> _sceneNodeLookup;
public:
	Scene(TinyObjectImporter& importer, VulkanDevice& device);
	~Scene();
	//	Mesh GetMesh() { return mesh; };
	SceneNode* GetSceneNode(std::string name) { return _sceneNodeLookup[name]; };
	void Update(float time);
	void Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);
};

