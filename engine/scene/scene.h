#pragma once
#include "../mesh/mesh.h"
#include "../importer/tinyobj_importer.h"
#include "scene_node.h"

class Scene
{
	TinyObjectImporter& importer;
	Mesh mesh;
	std::unordered_map<std::string, SceneNode*> sceneNodeLookup;
public:
	Scene(TinyObjectImporter& importer);
	~Scene();
	Mesh GetMesh() { return mesh; };
};

