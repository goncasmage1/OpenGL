#pragma once
#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"

struct Transform
{
private:

	Mat4 TransformationMat;

public:

	Transform();
	Transform(const Vec3& Pos, const Quat& Rot, const Vec3& Scl);
	friend const Transform Lerp(const Transform& From, const Transform& To, float progress);
	void UpdateTransformationMatrix(const Mat4 parentTransform);
	Mat4 GetTransformationMatrix();

	struct Vec3 Position;
	struct Quat Rotation;
	struct Vec3 Scale;
};

class Mesh
{

private:

	GLuint VaoId, VboId;

public:

	Mesh();
	Mesh(Transform transform);
	Mesh(Transform transform, Transform startTransform, Transform endTransform);

	bool TexcoordsLoaded, NormalsLoaded;
	std::vector <struct Vec3> Vertices, vertexData, Normals, normalData;
	std::vector <struct Vec2> Texcoords, texcoordData;
	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

	Transform transform;
	Transform startTransform;
	Transform endTransform;

	void SetAnimationProgress(float progress);
	Mat4 GetTransformationMatrix();
	void CreateBufferObjects();
	void DestroyBufferObjects();
	void Draw();

};