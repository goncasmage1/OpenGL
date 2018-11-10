#include "SceneNode.h"
#include "Shader/ShaderProgram.h"

SceneNode::SceneNode(std::shared_ptr<class Mesh> newMesh, std::shared_ptr<SceneNode> newParent, std::shared_ptr<ShaderProgram> newShaderProg) :
	childNodes(std::vector<std::shared_ptr<SceneNode>>()), transform(), mesh(newMesh), parent(newParent), shaderProg(newShaderProg), transformationMatrix(Mat4::IdentityMat())
{
	UpdateTransformationMatrix();
}

SceneNode::SceneNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform, std::shared_ptr<SceneNode> newParent, std::shared_ptr<ShaderProgram> newShaderProg) :
	childNodes(std::vector<std::shared_ptr<SceneNode>>()), transform(newTransform), mesh(newMesh), parent(newParent), shaderProg(newShaderProg), transformationMatrix(Mat4::IdentityMat())
{
	UpdateTransformationMatrix();
}

std::shared_ptr<SceneNode> SceneNode::CreateNode(std::shared_ptr<class Mesh> newMesh)
{
	std::shared_ptr<SceneNode> newChild = std::make_shared<SceneNode>(newMesh, shared_from_this(), shaderProg);
	childNodes.push_back(newChild);
	return newChild;
}

std::shared_ptr<SceneNode> SceneNode::CreateNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform)
{
	std::shared_ptr<SceneNode> newChild = std::make_shared<SceneNode>(newMesh, newTransform, shared_from_this(), shaderProg);
	childNodes.push_back(newChild);
	return newChild;
}

void SceneNode::UpdateTransformationMatrix()
{
	transformationMatrix = Mat4::ScaleMat(transform.Scale) * Mat4::TranslationMat(transform.Position) * transform.Rotation.GetMatrix();
	if (parent != nullptr) transformationMatrix *= parent->transformationMatrix;
	for (std::shared_ptr<SceneNode> node : childNodes)
	{
		node->UpdateTransformationMatrix();
	}
}

void SceneNode::Draw()
{
	shaderProg->Use();

	glUniformMatrix4fv(shaderProg->GetUniformId("ModelMatrix"), 1, GL_FALSE, transformationMatrix.GetData());

	mesh->Draw();
	for (std::shared_ptr<SceneNode> node : childNodes)
	{
		node->Draw();
	}
	glUseProgram(0);
}

std::vector<std::shared_ptr<SceneNode>> SceneNode::GetChildren()
{
	return childNodes;
}
