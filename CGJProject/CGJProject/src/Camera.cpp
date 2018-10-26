#include "Camera.h"

Camera::Camera()
{
	Offset = Vec3(0.5f, 0.5f, 0.5f);
	ModelMat = Mat4::IdentityMat();

	Direction = Vec3(1, 1, 1);
	Center = Vec3(0, 0, 0);	
	Eye = Vec3(-1, -1, -1);
	RightVector = Vec3(1, 0, 0);
	UpVector = Vec3(0, 1, 0);
	ViewMat = Mat4::ViewMat(Direction, UpVector);

	Orthographic = Mat4::OrthographicMat(0.01f, 100, -1, 1, -1, 1);
	Projection = Mat4::ProjectionMat(90, 640 / 480, 0.01f, 100);
}

void Camera::RotateCamera(Vec2 rotation)
{
	UpVector = RotateVector(UpVector, RightVector, rotation.y);
	RightVector = RotateVector(RightVector, UpVector, -rotation.x);
	Direction = Cross(RightVector, UpVector);

	ViewMat = Mat4::ViewMat(Direction, UpVector);
}

void Camera::MoveCamera(Vec3 movement)
{
	Offset -= Direction * movement.z;
	Offset += RightVector * movement.x;
	Offset -= UpVector * movement.y;

	ModelMat = Mat4::TranslationMat(Offset);
}
