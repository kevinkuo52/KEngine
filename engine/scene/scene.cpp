#include "scene.h"

Scene::Scene(TinyObjectImporter& importer) : importer(importer)
{
	mesh = importer.LoadModel();
	sceneNodeLookup.insert({ "level", new SceneNode("level", &mesh) });
	sceneNodeLookup.insert({ "camera", new SceneNode("camera") });
	sceneNodeLookup.insert({ "light", new SceneNode("light") });
}

Scene::~Scene()
{
}
