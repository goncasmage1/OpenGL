#pragma once

#include <iostream>
#include <vector>
#include "Math/Matrix.h"
#include "Mesh.h"

class SceneNode : public std::enable_shared_from_this<SceneNode>
{

protected:

	Transform transform;
	std::shared_ptr<class Mesh> mesh;

	std::shared_ptr<SceneNode> parent = nullptr;
	std::vector<std::shared_ptr<SceneNode>> childNodes;

public:

	SceneNode(std::shared_ptr<SceneNode> newParent);
	SceneNode(std::shared_ptr<class Mesh> newMesh, std::shared_ptr<SceneNode> newParent);
	SceneNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform, std::shared_ptr<SceneNode> newParent);

	std::shared_ptr<SceneNode> CreateNode(std::shared_ptr<class Mesh> newMesh);
	std::shared_ptr<SceneNode> CreateNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform);

	void SetTransform(Transform newTransform);
	void Draw(float* ViewMatrixData, float* ProjectionMatrixData);

	std::vector<std::shared_ptr<SceneNode>> GetChildren();

};