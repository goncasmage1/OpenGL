#pragma once
#include "Shader.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

class VertexShader : public Shader
{
public:
	VertexShader();

	virtual void Attach(GLuint ProgramId) override;
};