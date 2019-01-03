#pragma once

#include "ShaderProgram.h"
#include "..\Math\Vector.h"

class TextureShader : public ShaderProgram
{
private:
	GLuint textureId = 0;
	Vec4 plane;
	Vec3 lightPosition, lightColour;

public:

	TextureShader();

	void SetLightPosition(Vec3 lightPosition) { this->lightPosition = lightPosition; }
	void SetLightColour(Vec3 lightColour) { this->lightColour = lightColour;  }
	void SetTexture(const char* path);
	void SetTexture(unsigned int textureId) { this->textureId = textureId; }
	//void setTextureToFrameBuffer(unsigned int renderTexture);
	void Use();
};