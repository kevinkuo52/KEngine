#pragma once
#include "../mesh/mesh.h"
class SceneNode
{
	const std::string name;
	UniformBufferObject transformation;
	Mesh* mesh;
	SceneNode* parent;
	std::vector<SceneNode*> children;

public:
	SceneNode(std::string name, Mesh* mesh = nullptr, SceneNode* parent = nullptr, std::vector<SceneNode*> children = {});
	
	std::string GetName() { return name; };
	UniformBufferObject GetTransformation() { return transformation; };
	Mesh* GetMesh() { return mesh; };
	SceneNode* GetParent() { return parent; };
	std::vector<SceneNode*> GetChildren() { return children; };
};

