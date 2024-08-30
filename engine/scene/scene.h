#pragma once
#include "../mesh/mesh.h"
#include "../importer/tinyobj_importer.h"

class Scene
{
public:
	Scene(TinyObjectImporter& importer);
	~Scene();
	Mesh GetMesh() { return mesh; }
private:
	TinyObjectImporter& importer;
	Mesh mesh;
};

