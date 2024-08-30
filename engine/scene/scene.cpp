#include "scene.h"

Scene::Scene(TinyObjectImporter& importer) : importer(importer)
{
	mesh = importer.LoadModel();
}

Scene::~Scene()
{
}
