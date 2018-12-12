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

	bool isLoaded;

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();
private:
	unsigned char *image;
	GLuint textureID;
	int WIDTH, HEIGHT, BitDepth;
	std::string fileLocation;
};