#pragma once

#include <iostream>
#include <vector>
#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"
#include "Mesh.h"

struct Transform
{
private:

	Mat4 TransformationMat;

public:

	Transform();
	Transform(const Vec3& Pos, const Quat& Rot, const Vec3& Scl);
	friend const Transform Lerp(const Transform& From, const Transform& To, float progress);

	struct Vec3 Position;
	struct Quat Rotation;
	struct Vec3 Scale;
};

class SceneNode : public std::enable_shared_from_this<SceneNode>
{

protected:

	std::shared_ptr<class Mesh> mesh;
	std::shared_ptr<SceneNode> parent = nullptr;
	std::shared_ptr<class ShaderProgram> shaderProg = nullptr;

public:

	Transform transform;
	Mat4 transformationMatrix;

	std::vector<std::shared_ptr<SceneNode>> childNodes;

	SceneNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform, std::shared_ptr<SceneNode> newParent, std::shared_ptr<class ShaderProgram> newShaderProg);

	std::shared_ptr<SceneNode> CreateNode(std::shared_ptr<class Mesh> newMesh, Transform newTransform, std::shared_ptr<ShaderProgram> newShaderProg);

	void UpdateTransformationMatrix();
	void Draw();
	virtual void SetupUniforms();

};