#pragma once

#include "Math/Public/Vector.h"
#include "Math/Public/Matrix.h"

class Camera
{
protected:

	Vec3 Offset;
	Vec3 Center;
	Vec3 Eye;
	Vec3 Direction;
	Vec3 RightVector;
	Vec3 UpVector;

	Mat4 ModelMat;
	Mat4 ViewMat;

	Mat4 Orthographic;
	Mat4 Projection;

public:

	Camera();

	Mat4 GetProjectionMatrix(bool bUsePerspective) { return (bUsePerspective ? Projection : Orthographic); }
	Mat4 GetViewMatrix() { return ViewMat; }
	Mat4 GetModelMatrix() { return ModelMat; }

	void RotateCamera(Vec2 rotation);
	void MoveCamera(Vec3 movement);

};

