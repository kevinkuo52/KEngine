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

void Scene::Update(float time)
{
	_sceneNodeLookup["level"]->SetTransformation(glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
}

void Scene::Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout)
{
	//TODO properly traverse the graph
	_sceneNodeLookup["level"]->Draw(commandBuffer, pipelineLayout);
}
