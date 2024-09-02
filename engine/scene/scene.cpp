#include "scene.h"

Scene::Scene(TinyObjectImporter& importer, VulkanDevice& device) : _importer(importer), _device(device)
{
	std::shared_ptr<Mesh> mesh = importer.LoadModel();
	mesh->MeshUpload(_device);
	_sceneNodeLookup.insert({ "level", new SceneNode("level", mesh) });
	_sceneNodeLookup.insert({ "camera", new SceneNode("camera") });
	_sceneNodeLookup.insert({ "light", new SceneNode("light") });
}

Scene::~Scene()
{
}

void Scene::Draw(VkCommandBuffer commandBuffer)
{
	//TODO properly traverse the graph
	_sceneNodeLookup["level"]->GetMesh()->Draw(commandBuffer);
}
