#pragma once

#include "ShaderProgram.h"
#include "..\Math\Vector.h"

class TextureShader : public ShaderProgram
{
private:
	GLuint textureId = 0;
	Vec4 plane;

public:

	TextureShader();
	void SetTexture(const char* path);
	void SetTexture(unsigned int textureId) { this->textureId = textureId; }
	//void setTextureToFrameBuffer(unsigned int renderTexture);
	void Use();
};