#pragma once
#include <iostream>
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
private:

	GLuint ProgramId;
	const GLuint UBO_BP = 0;

	void CheckLinkage();

public:

	ShaderProgram(const std::vector<ShaderAttribute>& Attributes, const std::vector<std::string>& ShaderPaths);

	GLuint GetProgramId() const { return ProgramId; }
	GLuint GetUniformId(const GLchar* param) const { return glGetUniformLocation(ProgramId, param); }
	GLuint GetUboId(const GLchar* param) const { return glGetUniformBlockIndex(ProgramId, param); }
	const GLuint GetUBO_BP() const { return UBO_BP; }
	void Use();

	void Destroy();
};