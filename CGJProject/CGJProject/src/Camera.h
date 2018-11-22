#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

class Camera
{
protected:

	GLuint VboId;
	const GLuint UBO_BP = 0;

	int WinX, WinY;

	Vec3 Offset;
	Vec3 Direction;
	Vec3 RightVector;
	Vec3 UpVector;
	Vec2 Rotation;
	Quat Rotator;
	float Distance;

	bool bOrbiting = false;

	Mat4 ModelMat;
	Mat4 ViewMat;
	Mat4 Orthographic;
	Mat4 Projection;

public:

	Camera(int newWinX, int newWinY, int FOV);

	void CreateBufferObjects();
	void DestroyBufferObjects();

	Mat4 GetProjectionMatrix(bool bUsePerspective) { return (bUsePerspective ? Projection : Orthographic); }
	Mat4 GetViewMatrix() { return ViewMat; }
	void SetPerspectiveMatrix(float FOV, float aspectRation, float near, float far);
	void SetOrthographicMatrix(float n, float f, float b, float t, float r, float l);
	bool IsOrbiting() { return bOrbiting; }
	void ToggleOrbiting() { bOrbiting = !bOrbiting; }

	void RotateCamera(Vec2 rotation);
	void Zoom(float amount);

};

