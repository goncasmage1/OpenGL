#include "SceneNode.h"

SceneNode::SceneNode(std::shared_ptr<class Mesh> newMesh) :
	childNodes(std::vector<std::shared_ptr<SceneNode>>()), transform(), mesh(newMesh)
{
}

SceneNode::SceneNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform) :
	childNodes(std::vector<std::shared_ptr<SceneNode>>()), transform(newTransform), mesh(newMesh)
{
}

std::shared_ptr<SceneNode> SceneNode::CreateNode(std::shared_ptr<class Mesh> newMesh)
{
	std::shared_ptr<SceneNode> newChild = std::make_shared<SceneNode>(newMesh);
	childNodes.push_back(newChild);
	return newChild;
}

std::shared_ptr<SceneNode> SceneNode::CreateNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform)
{
	std::shared_ptr<SceneNode> newChild = std::make_shared<SceneNode>(newMesh, newTransform);
	childNodes.push_back(newChild);
	return newChild;
}

void SceneNode::SetTransform(Transform newTransform)
{
	transform = newTransform;
}

std::vector<std::shared_ptr<SceneNode>> SceneNode::GetChildren()
{
	return childNodes;
}
