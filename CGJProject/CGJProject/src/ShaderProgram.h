#pragma once
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

class ShaderProgram
{
	GLuint ProgramId;

public:

	ShaderProgram();

	GLuint GetProgramId() const { return ProgramId; }
	GLuint GetUniformId(std::string param) const { return glGetUniformLocation(ProgramId, "Matrix"); }

	void Destroy();
};