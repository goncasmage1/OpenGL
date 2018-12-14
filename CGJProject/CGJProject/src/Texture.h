#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "GL/glew.h"

class Texture
{
public:
	Texture();
	Texture(std::string fileLoc);

	unsigned char *image;
	int WIDTH, HEIGHT, BitDepth;
	bool isLoaded;

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();
private:
	GLuint textureID;
	std::string fileLocation;
};