#include "SceneNode.h"
#include "Shader/ShaderProgram.h"

/////////////
//	Transform
/////////////
Transform::Transform() :
	Position(Vec3(0.f)), Rotation(FromAngleAxis(Vec4(0.f, 1.f, 0.f, 1.f), 45.f)), Scale(Vec3(1.f))
{
	TransformationMat = Mat4::ScaleMat(Scale) * Mat4::TranslationMat(Position) * Rotation.GetMatrix();
}

Transform::Transform(const Vec3 & Pos, const Quat & Rot, const Vec3 & Scl) :
	Position(Pos), Rotation(Rot), Scale(Scl)
{
	TransformationMat = Mat4::ScaleMat(Scale) * Mat4::TranslationMat(Position) * Rotation.GetMatrix();
}

const Transform Lerp(const Transform & From, const Transform & To, float progress)
{
	Transform Lerped = Transform();
	Lerped.Position = Lerp(From.Position, To.Position, progress);
	Lerped.Rotation = Lerp(From.Rotation, To.Rotation, progress);
	Lerped.Scale = Lerp(From.Scale, To.Scale, progress);
	return Lerped;
}

void Transform::UpdateTransformationMatrix(const Mat4 parentTransform)
{
	TransformationMat = Mat4::ScaleMat(Scale) * Mat4::TranslationMat(Position) * Rotation.GetMatrix();
	TransformationMat *= parentTransform;
}

Mat4 Transform::GetTransformationMatrix()
{
	return TransformationMat;
}


/////////////
//	SceneNode
/////////////

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

void SceneNode::SetAnimationProgress(float progress)
{
	transform = Lerp(startTransform, endTransform, progress);
	UpdateTransformationMatrix();
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
