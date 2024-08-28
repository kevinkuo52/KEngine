#pragma once

#include <vector>
#include <string>

#include "../mesh/mesh.h"

class TinyObjectImporter
{


public:
	const std::string MODEL_PATH = "application/scene_asset/model/spheres.obj";
	const std::string TEXTURE_PATH = "application/scene_asset/model/white_stone.png";

	TinyObjectImporter();
	void LoadModel(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
};
