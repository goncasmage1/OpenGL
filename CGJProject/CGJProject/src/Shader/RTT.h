#pragma once
#include "ShaderProgram.h"
#include "..\Math\Vector.h"

class RTT: public ShaderProgram
{
private:
	GLuint textureId = 0;

public:

	RTT(GLuint texture);
	void SetTexture(unsigned int textureId) { this->textureId = textureId; }
	void Use();
};