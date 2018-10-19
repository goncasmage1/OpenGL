#pragma once
#include <string>
#include <vector>

#include "GL/glew.h"
#include "GL/freeglut.h"

struct ShaderAttribute
{
	int index;
	const GLchar* name;
};

class ShaderProgram
{
	GLuint ProgramId;

public:

	ShaderProgram(std::vector<ShaderAttribute> Attributes);

	GLuint GetProgramId() const { return ProgramId; }
	GLuint GetUniformId(std::string param) const { return glGetUniformLocation(ProgramId, "Matrix"); }

	void Destroy();
};