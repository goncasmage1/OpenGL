#include "Camera.h"

Camera::Camera(int newWinX, int newWinY, int FOV)
{
	WinX = newWinX;
	WinY = newWinY;

	bOrbiting = false;

	ModelMat = Mat4::IdentityMat();

	////
	Yaw = -90.0f;
	Pitch = 0.0f;

	Offset = Vec3(0.0, 0.0, 0.0);
	updateCamera();

	Near = 0.1f;
	Far = 300.f;
	////
	/*RightVector = Vec3(1, 0, 0);
	UpVector = Vec3(0, 1, 0);
	Direction = Cross(RightVector, UpVector);
	Rotation = Vec2();
	Rotator = Quat(1, 0, 0, 0);
	Distance = 3.f;
	Offset = Vec3(Distance);*/
	//Mat4 RotMat = Rotator.GetMatrix();
	//ViewMat = bOrbiting ? Mat4::TranslationMat(Vec3(0, 0, -Distance)) * RotMat : Mat4::ViewMat(Direction, UpVector);

	Orthographic = Mat4::OrthographicMat(0.01f, 100, -1, 1, -1, 1);
	FOV = FOV * (float) 0.0174532925;
	Projection = Mat4::ProjectionMat(FOV, (float)WinX / (float)WinY, Near, Far);
}

void Camera::updateCamera()
{
	float YawRadians = Yaw * (float) 0.0174532925;
	float PitchRadians = Pitch * (float) 0.0174532925;
	Direction.x = cos(YawRadians) * cos(PitchRadians);
	Direction.y = sin(PitchRadians);
	Direction.z = sin(YawRadians) * cos(PitchRadians);
	Direction.Normalize();

	RightVector = Cross(Direction, Vec3(0.0, 1.0, 0.0));
	RightVector.Normalize();
	UpVector = Cross(RightVector, Direction);
	UpVector.Normalize();
	ViewMat = Mat4::ViewMatrix(Direction, UpVector, RightVector, Offset);
}

void Camera::CreateBufferObjects()
{
	glGenBuffers(1, &VboId);
	glBindBuffer(GL_UNIFORM_BUFFER, VboId);
	{
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Mat4) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, VboId);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), ViewMat.GetData());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mat4), sizeof(Mat4), Projection.GetData());
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
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mat4), sizeof(Mat4), Projection.GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::SetOrthographicMatrix(float n, float f, float b, float t, float r, float l)
{
	Orthographic = Mat4::OrthographicMat(0.01f, 100, -1, 1, -1, 1);
}

void Camera::RotateCamera(Vec2 rotation)
{
	if (rotation == Vec2(0.f, 0.f)) return;
	//if (!bOrbiting) rotation *= 50000.f;

	Yaw += rotation.x;
	Pitch += rotation.y;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	updateCamera();

	/*UpVector = RotateVector(UpVector, RightVector, rotation.y);
	RightVector = RotateVector(RightVector, UpVector, -rotation.x);
	Direction = Cross(RightVector, UpVector);

	UpVector.Normalize();
	RightVector.Normalize();
	Direction.Normalize();
	/*Rotator = FromAngleAxis(Vec4::X(), rotation.y) * FromAngleAxis(Vec4::Y(), rotation.x) * Rotator;
	Mat4 RotMat = bOrbiting ? Rotator.GetMatrix() : Mat4::RotationMat(RightVector, Rotation.y) * Mat4::RotationMat(UpVector, -Rotation.x);
	
	ViewMat = bOrbiting ? Mat4::TranslationMat(Vec3(0, 0, -Distance)) * RotMat : Mat4::ViewMatrix(Direction, UpVector, Offset);
	/*glBindBuffer(GL_UNIFORM_BUFFER, VboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), ViewMat.GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);*/
}
/*
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
*/
Vec3 Camera::GetDirection()
{
	return Direction;
}

Vec3 Camera::GetUp()
{
	return UpVector;
}

Vec3 Camera::GetOffset()
{
	return Offset;
}


void Camera::FlipView()
{	
	Pitch = -Pitch;
	Offset.y = -Offset.y;
	updateCamera();

	ViewMat = Mat4::ViewMatrix(Direction, UpVector, RightVector, Offset);
	glBindBuffer(GL_UNIFORM_BUFFER, VboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), ViewMat.GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void Camera::SetModelMat(Mat4 mat)
{
	ViewMat = mat;
}

void Camera::MoveCamera(Vec3 movement)
{
	Offset += Direction * movement.z;
	Offset += RightVector * movement.x;
	Offset += UpVector * movement.y;

	ViewMat = Mat4::ViewMatrix(Direction, UpVector, RightVector, Offset);
	glBindBuffer(GL_UNIFORM_BUFFER, VboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), ViewMat.GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}