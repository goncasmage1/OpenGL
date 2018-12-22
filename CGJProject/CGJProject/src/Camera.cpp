#include "Camera.h"

Camera::Camera(int newWinX, int newWinY, int FOV)
{
	WinX = newWinX;
	WinY = newWinY;

	bOrbiting = false;

	ModelMat = Mat4::IdentityMat();

	RightVector = Vec3(1, 0, 0);
	UpVector = Vec3(0, 1, 0);
	Direction = Cross(RightVector, UpVector);
	Rotation = Vec2();
	Rotator = Quat(1, 0, 0, 0);
Distance = 3.f;
Offset = Vec3(Distance);
Mat4 RotMat = Rotator.GetMatrix();
ViewMat = bOrbiting ? Mat4::TranslationMat(Vec3(0, 0, -Distance)) * RotMat : Mat4::ViewMat(Direction, UpVector);

Orthographic = Mat4::OrthographicMat(0.01f, 100, -1, 1, -1, 1);
Projection = Mat4::ProjectionMat(FOV, (float)WinX / (float)WinY, 0.01f, 500);
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


std::vector<Vec3> Camera::FlipView()
{
	std::vector<Vec3> result = { Direction, UpVector, RightVector };

	Direction.y = -Direction.y;
	Direction = Normalized(Direction);

	//UpVector = Normalized(Cross(Direction, RightVector));

	UpVector.x = -UpVector.x;
	UpVector.z = -UpVector.z;
	UpVector = Normalized(UpVector);

	RightVector = Cross(UpVector, Direction);
	ViewMat = Mat4({{
			{RightVector.x, RightVector.y, RightVector.z, 0},
			{UpVector.x, UpVector.y, UpVector.z, 0},
			{-Direction.x, -Direction.y, -Direction.z, 0},
			{0, 0, 0, 1}}});

/*	float Px = Offset.x - (Offset.y / Direction.y)*Direction.x;
	Vec3 P = Vec3(Px, 0.f, Offset.z); //Plane is at origin

	Offset.y = -Offset.y;
	Direction = Normalized(P - Offset);
	UpVector = Normalized(Cross(Direction, RightVector));*/

	ViewMat = Mat4::ViewMat(Direction, UpVector);
	glBindBuffer(GL_UNIFORM_BUFFER, VboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), ViewMat.GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Offset.y = -Offset.y;
	ModelMat = Mat4::TranslationMat(Offset);
	return result;
}

void Camera::UnflipView(std::vector<Vec3> preSettings)
{
	/*float dotAngle = Dot(Vec3(0.0f, 1.0f, 0.0f), Normalized(UpVector));
	dotAngle = -dotAngle + 1;

	float dotFront = Dot(Vec3(0.f, 1.f, 0.f), Normalized(Direction));
	float pitch = dotFront > 0 ? -dotAngle * 90.f : dotAngle * 90.f;

	UpVector = RotateVector(UpVector, RightVector, pitch);
	//RightVector = RotateVector(RightVector, UpVector, -rotation.x);
	Direction = Cross(RightVector, UpVector);

	UpVector = Normalized(UpVector);
	Direction = Normalized(Direction);*/
	
	Offset.y = -Offset.y;
	Direction = preSettings[0];
	UpVector = preSettings[1];
	RightVector = preSettings[2];

	ViewMat = Mat4::ViewMat(Direction, UpVector);
	glBindBuffer(GL_UNIFORM_BUFFER, VboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), ViewMat.GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	ModelMat = Mat4::TranslationMat(Offset);
}


void Camera::SetModelMat(Mat4 mat)
{
	ViewMat = mat;
}

void Camera::MoveCamera(Vec3 movement)
{
	Offset -= Direction * movement.z;
	Offset += RightVector * movement.x;
	Offset -= UpVector * movement.y;

	ModelMat = Mat4::TranslationMat(Offset);
}