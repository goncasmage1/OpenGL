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
	std::vector<const char*> faces = {
	"assets/Textures/sea/right.jpg",
	"assets/Textures/sea/left.jpg",
	"assets/Textures/sea/top.jpg",
	"assets/Textures/sea/bottom.jpg",
	"assets/Textures/sea/back.jpg",
	"assets/Textures/sea/front.jpg"
	};

public:

	SkyboxShader(Mat4 view);
	GLuint GetTexture();
	void SetViewMatrix(Mat4 view);
	void LoadCubeMap(std::vector<const char*> faces);
	void Use();
};