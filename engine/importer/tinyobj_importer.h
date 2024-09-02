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
	std::shared_ptr<Mesh> LoadModel();
};
