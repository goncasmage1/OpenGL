#include "SceneNode.h"
#include "Shader/ShaderProgram.h"

/////////////
//	Transform
/////////////
Transform::Transform() :
	Position(Vec3(0.f)), Rotation(Quat()), Scale(Vec3(1.f))
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


/////////////
//	SceneNode
/////////////

SceneNode::SceneNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform, std::shared_ptr<SceneNode> newParent, std::shared_ptr<ShaderProgram> newShaderProg) :
	childNodes(std::vector<std::shared_ptr<SceneNode>>()), transform(newTransform), mesh(newMesh), parent(newParent), shaderProg(newShaderProg), transformationMatrix(Mat4::IdentityMat())
{
	UpdateTransformationMatrix();
}

std::shared_ptr<SceneNode> SceneNode::CreateNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform, std::shared_ptr<ShaderProgram> newShaderProg)
{
	std::shared_ptr<SceneNode> newChild = std::make_shared<SceneNode>(newMesh, newTransform, shared_from_this(), newShaderProg);
	childNodes.push_back(newChild);
	return newChild;
}

void SceneNode::UpdateTransformationMatrix()
{
	Vec3 tempPos = transform.Position * transform.Scale;
	tempPos.z = transform.Position.z;
	transformationMatrix = Mat4::TranslationMat(tempPos) * transform.Rotation.GetMatrix() * Mat4::ScaleMat(transform.Scale);
	if (parent != nullptr) transformationMatrix = parent->transformationMatrix * transformationMatrix;
	for (std::shared_ptr<SceneNode> node : childNodes)
	{
		node->UpdateTransformationMatrix();
	}
}

void SceneNode::Draw(Vec4 plane)
{
	if (shaderProg != nullptr && mesh != nullptr)
	{

		shaderProg->SetPlane(plane);
		shaderProg->Use();
		SetupUniforms();
		mesh->Draw();
		glDepthMask(GL_TRUE);
	}

	for (std::shared_ptr<SceneNode> node : childNodes)
	{
		node->Draw(plane);
	}
	glUseProgram(0);
}

void SceneNode::SetupUniforms()
{
	glUniformMatrix4fv(shaderProg->GetUniformId("ModelMatrix"), 1, GL_FALSE, transformationMatrix.GetData());
}
