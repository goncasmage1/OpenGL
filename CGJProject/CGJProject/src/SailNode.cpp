#include "SailNode.h"
#include "SailMesh.h"

SailNode::SailNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform, std::shared_ptr<SceneNode> newParent, std::shared_ptr<class ShaderProgram> newShaderProg) :
	SceneNode(newMesh, newTransform, newParent, newShaderProg)
{
	sailMesh = std::dynamic_pointer_cast<SailMesh>(newMesh);
	UpdateClothTransform();
}

void SailNode::UpdateTransformationMatrix()
{
	UpdateClothTransform();
	SceneNode::UpdateTransformationMatrix();
}

void SailNode::UpdateClothTransform()
{
	if (sailMesh != nullptr)
	{
		sailMesh->cloth->setTranslation(ToPhysxVec(transform.Position));
		sailMesh->cloth->setRotation(ToPhysxQuat(transform.Rotation));
	}
}
