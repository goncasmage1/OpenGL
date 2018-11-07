#pragma once

#include <iostream>
#include <vector>
#include "Math/Matrix.h"
#include "Mesh.h"

class SceneNode
{

protected:

	Transform transform;
	std::shared_ptr<class Mesh> mesh;

	std::vector<std::shared_ptr<SceneNode>> childNodes;

public:

	SceneNode(std::shared_ptr<class Mesh> newMesh);
	SceneNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform);

	std::shared_ptr<SceneNode> CreateNode(std::shared_ptr<class Mesh> newMesh);
	std::shared_ptr<SceneNode> CreateNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform);

	void SetTransform(Transform newTransform);

	std::vector<std::shared_ptr<SceneNode>> GetChildren();

};