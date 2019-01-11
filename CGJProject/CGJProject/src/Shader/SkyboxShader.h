#pragma once

#include "ShaderProgram.h"
#include "SOIL.h"
#include "../Math/Matrix.h"
#include <vector>

class SkyboxShader : public ShaderProgram
{
private:
	GLuint textureId = 0;
	int width, height;
	Mat4 view;

public:

	SkyboxShader();
	GLuint GetTexture();
	void SetViewMatrix(Mat4 view);
	void LoadCubeMap(std::vector<const char*> faces);
	void Use();
};