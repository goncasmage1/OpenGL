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
	const GLuint UBO_BP = 0;

public:

	ShaderProgram(std::vector<ShaderAttribute> Attributes);

	GLuint GetProgramId() const { return ProgramId; }
	GLuint GetUniformId(const GLchar* param) const { return glGetUniformLocation(ProgramId, param); }
	GLuint GetUboId(const GLchar* param) const { return glGetUniformBlockIndex(ProgramId, param); }
	const GLuint GetUBO_BP() const { return UBO_BP; }

	void Destroy();
};