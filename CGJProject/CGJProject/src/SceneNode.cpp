#include "SceneNode.h"

SceneNode::SceneNode(std::shared_ptr<class Mesh> newMesh, std::shared_ptr<SceneNode> newParent) :
	childNodes(std::vector<std::shared_ptr<SceneNode>>()), transform(), mesh(newMesh), parent(newParent)
{
}

SceneNode::SceneNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform, std::shared_ptr<SceneNode> newParent) :
	childNodes(std::vector<std::shared_ptr<SceneNode>>()), transform(newTransform), mesh(newMesh), parent(newParent)
{
}

std::shared_ptr<SceneNode> SceneNode::CreateNode(std::shared_ptr<class Mesh> newMesh)
{
	std::shared_ptr<SceneNode> newChild = std::make_shared<SceneNode>(newMesh, shared_from_this());
	childNodes.push_back(newChild);
	return newChild;
}

std::shared_ptr<SceneNode> SceneNode::CreateNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform)
{
	std::shared_ptr<SceneNode> newChild = std::make_shared<SceneNode>(newMesh, newTransform, shared_from_this());
	childNodes.push_back(newChild);
	return newChild;
}

void SceneNode::SetTransform(Transform newTransform)
{
	transform = newTransform;
}

void SceneNode::Draw(float * ViewMatrixData, float * ProjectionMatrixData)
{
	mesh->Draw(ViewMatrixData, ProjectionMatrixData);
	for (std::shared_ptr<SceneNode> node : childNodes)
	{
		node->Draw(ViewMatrixData, ProjectionMatrixData);
	}
}

std::vector<std::shared_ptr<SceneNode>> SceneNode::GetChildren()
{
	return childNodes;
}
