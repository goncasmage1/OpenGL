#include "Camera.h"

Camera::Camera()
{
	Offset = Vec3();
	ModelMat = Mat4::IdentityMat();

	Direction = Vec3(0, 0, 4);
	RightVector = Vec3(1, 0, 0);
	UpVector = Vec3(0, 1, 0);
	ViewMat = Mat4::ViewMat(Direction, UpVector);

	Orthographic = Mat4::OrthographicMat(0.01f, 100, -1, 1, -1, 1);
	Projection = Mat4::ProjectionMat(90, 640 / 480, 0.01f, 100);

	Rotation = Vec2();
	Rotator = Quat(1, 0, 0, 0);
}

void Camera::RotateCamera(Vec2 rotation)
{
	if (!bUseQuaternion)
	{
		Rotation.x -= rotation.x;
		Rotation.y += rotation.y;
	}
	else Rotator = FromAngleAxis(Vec4::X(), rotation.y) * FromAngleAxis(Vec4::Y(), rotation.x) * Rotator;

	Mat4 RotMat = bUseQuaternion ? Rotator.GetMatrix() : Mat4::RotationMat(RightVector, Rotation.y) * Mat4::RotationMat(UpVector, -Rotation.x);

	ViewMat = Mat4::TranslationMat(Vec3(0, 0, -Distance)) * RotMat;
}

void Camera::MoveCamera(Vec3 movement)
{
	if (bOrbiting) return;

	Offset -= Direction * movement.z;
	Offset += RightVector * movement.x;
	Offset -= UpVector * movement.y;
}

void Camera::Zoom(float amount)
{
	if (amount == 0.f) return;

	std::cout << Distance << std::endl;

	if (amount < 0) Distance /= -amount;
	else Distance *= amount;

	if (!bOrbiting) Offset -= Direction * amount;
}
