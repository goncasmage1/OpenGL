#pragma once

#include "Math/Public/Vector.h"
#include "Math/Public/Matrix.h"
#include "Math/Public/Quaternion.h"

class Camera
{
protected:

	Vec3 Offset;

	Vec3 Direction;
	Vec3 RightVector;
	Vec3 UpVector;
	Quat Rotator;
	float Distance = 3.f;

	bool bOrbiting = false;

	Mat4 ModelMat;
	Mat4 ViewMat;
	Mat4 Orthographic;
	Mat4 Projection;

public:

	Camera();

	Mat4 GetProjectionMatrix(bool bUsePerspective) { return (bUsePerspective ? Projection : Orthographic); }
	Mat4 GetViewMatrix() { return ViewMat; }
	Mat4 GetModelMatrix() { return ModelMat; }
	bool IsOrbiting() { return bOrbiting; }
	void ToggleOrbiting() { bOrbiting = !bOrbiting; }

	void RotateCamera(Vec2 rotation);
	void MoveCamera(Vec3 movement);
	void Zoom(float amount);

};

