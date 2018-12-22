#pragma once

#include "ShaderProgram.h"
#include "SOIL.h"
#include <vector>

class SkyboxShader : public ShaderProgram
{
private:
	GLuint textureId = 0;
	int width, height;

public:

	SkyboxShader();
	GLuint GetTexture();
	void LoadCubeMap(std::vector<const char*> faces);
	void Use();
};