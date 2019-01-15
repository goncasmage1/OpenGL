#pragma once
#include <memory>
#include "SceneNode.h"


class WaterRenderer: 
	public SceneNode {

public:
	WaterRenderer(std::shared_ptr<class Mesh> newMesh, Transform newTransform,
		std::shared_ptr<SceneNode> newParent, std::shared_ptr<class ShaderProgram> newShaderProg) :
		SceneNode(newMesh, newTransform, newParent, newShaderProg) {};

	void Draw(Vec4 plane);
};