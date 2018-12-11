#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "GL/glew.h"
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(std::string fileLoc);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();
private:
	GLuint textureID;
	int WIDTH, HEIGHT, BitDepth;
	std::string fileLocation;
};