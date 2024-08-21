#pragma once

#include <vector>
#include <string>

#include "../mesh/mesh.h"

class TinyObjectImporter
{
	const std::string MODEL_PATH = "application/scene_asset/model/viking_room.obj";
	const std::string TEXTURE_PATH = "application/scene_asset/model/viking_room.png";

public:
	TinyObjectImporter();
	void LoadModel(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
};
