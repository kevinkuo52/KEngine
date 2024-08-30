#include "scene.h"

Scene::Scene(TinyObjectImporter& importer) : importer(importer)
{
	mesh = importer.LoadModel();
	sceneNodeLookup.insert({ "level", &SceneNode("level", &mesh) });
	sceneNodeLookup.insert({ "camera", &SceneNode("camera") });
	sceneNodeLookup.insert({ "light", &SceneNode("light") });
}

Scene::~Scene()
{
}
