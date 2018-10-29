#include "Camera.h"

Camera::Camera()
{
	Offset = Vec3();
	ModelMat = Mat4::IdentityMat();

	Direction = Vec3(1, 1, 1);
	Center = Vec3();
	Eye = Vec3(-1, -1, -1);
	RightVector = Vec3(1, 0, 0);
	UpVector = Vec3(0, 1, 0);
	ViewMat = Mat4::ViewMat(Direction, UpVector);

	Orthographic = Mat4::OrthographicMat(0.01f, 100, -1, 1, -1, 1);
	Projection = Mat4::ProjectionMat(90, 640 / 480, 0.01f, 100);

	Rotator = Quat();
}

void Camera::RotateCamera(Vec2 rotation)
{
	UpVector.Rotate(RightVector, rotation.y);
	RightVector.Rotate(UpVector, -rotation.x);
	Direction = Cross(RightVector, UpVector);
	Eye = -Direction;

	Rotator *= Quat(-rotation.x, UpVector);
	Rotator *= Quat(rotation.y, RightVector);

	ViewMat = Mat4::ViewMat(Direction, UpVector);
	//ViewMat = Rotator.GetMatrix();

	if (bOrbiting)
	{
		/*Offset = Vec3(Direction.x, -Direction.y, Direction.z) * (Distance + Normalized(Direction));
		ModelMat = Mat4::TranslationMat(Offset);*/
	}
}

void Camera::MoveCamera(Vec3 movement)
{
	if (bOrbiting) return;

	Offset -= Direction * movement.z;
	Offset += RightVector * movement.x;
	Offset -= UpVector * movement.y;

	ModelMat = Mat4::TranslationMat(Offset);
}

void Camera::Zoom(float amount)
{
	if (amount == 0.f) return;

	std::cout << Distance << std::endl;

	if (amount < 0) Distance /= -amount;
	else Distance *= amount;

	if (!bOrbiting) Offset -= Direction * amount;
}
