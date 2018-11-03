#pragma once
#include <vector>
#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"

struct Transform
{
	Transform();
	Transform(const Vec3& Pos, const Quat& Rot, const Vec3& Scl);
	friend const Transform Lerp(const Transform& From, const Transform& To, float progress);

	struct Vec3 Position;
	struct Quat Rotation;
	struct Vec3 Scale;
};

class Mesh
{

protected:

	struct Mat4 TransformationMat = Mat4();

public:

	bool TexcoordsLoaded, NormalsLoaded;
	std::vector <struct Vec3> Vertices, vertexData, Normals, normalData;
	std::vector <struct Vec2> Texcoords, texcoordData;
	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

	Transform transform = Transform();
	Transform startTransform = Transform();
	Transform endTransform = Transform();

	void SetAnimationProgress(float progress);
	Mat4 GetTransformationMatrix();

};