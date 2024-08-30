#include "scene_node.h"

SceneNode::SceneNode(std::string name, Mesh* mesh = nullptr, SceneNode* parent = nullptr, std::vector<SceneNode*> children = {}) 
	: name(name), mesh(mesh), parent(parent), children(children)
{
}
