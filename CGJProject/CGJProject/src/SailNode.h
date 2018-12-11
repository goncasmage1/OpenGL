#pragma once

#include "SceneNode.h"

class SailNode : public SceneNode
{
protected:

	std::shared_ptr<class SailMesh> sailMesh = nullptr;

	void UpdateClothTransform();

public:

	SailNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform, std::shared_ptr<SceneNode> newParent, std::shared_ptr<class ShaderProgram> newShaderProg);
	
	virtual void UpdateTransformationMatrix() override;
};

