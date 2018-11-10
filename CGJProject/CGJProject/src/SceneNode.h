#pragma once

#include <iostream>
#include <vector>
#include "Math/Matrix.h"
#include "Mesh.h"

class SceneNode : public std::enable_shared_from_this<SceneNode>
{

protected:

	std::shared_ptr<class Mesh> mesh;

	std::shared_ptr<SceneNode> parent = nullptr;
	std::shared_ptr<class ShaderProgram> shaderProg = nullptr;
	std::vector<std::shared_ptr<SceneNode>> childNodes;

public:

	Transform transform;
	Mat4 transformationMatrix;

	SceneNode(std::shared_ptr<class Mesh> newMesh, std::shared_ptr<SceneNode> newParent, std::shared_ptr<class ShaderProgram> newShaderProg);
	SceneNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform, std::shared_ptr<SceneNode> newParent, std::shared_ptr<class ShaderProgram> newShaderProg);

	std::shared_ptr<SceneNode> CreateNode(std::shared_ptr<class Mesh> newMesh);
	std::shared_ptr<SceneNode> CreateNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform);

	void UpdateTransformationMatrix();
	void Draw();

	std::vector<std::shared_ptr<SceneNode>> GetChildren();

};