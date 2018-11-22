#include "Camera.h"

Camera::Camera(int newWinX, int newWinY, int FOV)
{
	WinX = newWinX;
	WinY = newWinY;

	bOrbiting = false;

	Offset = Vec3(5.f);
	ModelMat = Mat4::IdentityMat();

	Direction = Vec3(1, 1, 1);
	RightVector = Vec3(1, 0, 0);
	UpVector = Vec3(0, 1, 0);
	Rotation = Vec2();
	Rotator = Quat(1, 0, 0, 0);
	Distance = 3.f;
	Mat4 RotMat = Rotator.GetMatrix();
	ViewMat = bOrbiting ? Mat4::TranslationMat(Vec3(0, 0, -Distance)) * RotMat : Mat4::ViewMat(Direction, UpVector);

	Orthographic = Mat4::OrthographicMat(0.01f, 100, -1, 1, -1, 1);
	Projection = Mat4::ProjectionMat(FOV, WinX / WinY, 0.01f, 100);
}

void Camera::CreateBufferObjects()
{
	glGenBuffers(1, &VboId);
	glBindBuffer(GL_UNIFORM_BUFFER, VboId);
	{
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Mat4) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, VboId);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), GetViewMatrix().GetData());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mat4), sizeof(Mat4), GetProjectionMatrix(true).GetData());
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::DestroyBufferObjects()
{
	glDeleteBuffers(1, &VboId);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::SetPerspectiveMatrix(float newFOV, float aspectRation, float newNear, float newFar)
{
	Projection = Mat4::ProjectionMat(newFOV, aspectRation, newNear, newFar);
	glBindBuffer(GL_UNIFORM_BUFFER, VboId);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mat4), sizeof(Mat4), GetProjectionMatrix(true).GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::SetOrthographicMatrix(float n, float f, float b, float t, float r, float l)
{
	Orthographic = Mat4::OrthographicMat(0.01f, 100, -1, 1, -1, 1);
}

void Camera::RotateCamera(Vec2 rotation)
{
	if (rotation == Vec2(0.f, 0.f)) return;
	if (!bOrbiting) rotation *= 50000.f;

	UpVector = RotateVector(UpVector, RightVector, rotation.y);
	RightVector = RotateVector(RightVector, UpVector, -rotation.x);
	Direction = Cross(RightVector, UpVector);

	Rotator = FromAngleAxis(Vec4::X(), rotation.y) * FromAngleAxis(Vec4::Y(), rotation.x) * Rotator;
	Mat4 RotMat = bOrbiting ? Rotator.GetMatrix() : Mat4::RotationMat(RightVector, Rotation.y) * Mat4::RotationMat(UpVector, -Rotation.x);
	ViewMat = bOrbiting ? Mat4::TranslationMat(Vec3(0, 0, -Distance)) * RotMat : Mat4::ViewMat(Direction, UpVector);
	glBindBuffer(GL_UNIFORM_BUFFER, VboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), ViewMat.GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::Zoom(float amount)
{
	if (amount == 0.f) return;

	if (amount < 0) Distance /= -amount;
	else Distance *= amount;

	if (!bOrbiting) Offset -= Direction * amount;

	Mat4 RotMat = Rotator.GetMatrix();

	ViewMat = Mat4::TranslationMat(Vec3(0, 0, -Distance)) * RotMat;
	glBindBuffer(GL_UNIFORM_BUFFER, VboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), ViewMat.GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::MoveCamera(Vec3 movement)
{
	Offset -= Direction * movement.z;
	Offset += RightVector * movement.x;
	Offset -= UpVector * movement.y;

	ModelMat = Mat4::TranslationMat(Offset);
}