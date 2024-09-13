#include "scene.h"
#include <stack>

Scene::Scene(TinyObjectImporter& importer, VulkanDevice& device) : _importer(importer), _device(device)
{
	_root = new SceneNode("level");
	std::shared_ptr<Mesh> mesh = importer.LoadModel();
	mesh->MeshUpload(_device);
	_sceneNodeLookup.insert({ "obj1", new SceneNode("obj1", mesh) });
	_root->AddChild(_sceneNodeLookup["obj1"]);
	std::shared_ptr<Mesh> mesh2 = importer.LoadModel();
	mesh2->MeshUpload(_device);
	_sceneNodeLookup.insert({ "obj2", new SceneNode("obj2", mesh) });
	_sceneNodeLookup["obj1"]->AddChild(_sceneNodeLookup["obj2"]);
	
	_sceneNodeLookup.insert({ "camera", new SceneNode("camera") });
	_root->AddChild(_sceneNodeLookup["camera"]);

	_sceneNodeLookup.insert({ "light", new SceneNode("light") });
	_root->AddChild(_sceneNodeLookup["light"]);
}

Scene::~Scene()
{

}

void Scene::Update(float time)
{
	_sceneNodeLookup["obj1"]->SetTransformation(glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	_sceneNodeLookup["obj2"]->SetTransformation(glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Scene::Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout)
{
	std::stack<SceneNode*> stack({ _root });
	
	while (!stack.empty()) {
		SceneNode* curr = stack.top();
		curr->Draw(commandBuffer, pipelineLayout);
		stack.pop();
		for (SceneNode* node : curr->GetChildren()) {
			stack.push(node);
		}
	}
}
