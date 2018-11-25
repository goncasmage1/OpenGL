#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "GL/freeglut.h"

struct ShaderAttribute
{
	ShaderAttribute(int newIndex, const GLchar* newName);
	int index;
	const GLchar* name;
};

class ShaderProgram
{
protected:

	GLuint ProgramId;
	const GLuint UBO_BP = 0;

	void BuildShader(const std::vector<ShaderAttribute> Attributes, const std::vector<std::string> ShaderPaths);
	void CheckLinkage();

public:

	ShaderProgram();
	ShaderProgram(const std::vector<ShaderAttribute> Attributes, const std::vector<std::string> ShaderPaths);

	GLuint GetProgramId() const { return ProgramId; }
	GLuint GetUniformId(const GLchar* param) const { return glGetUniformLocation(ProgramId, param); }
	GLuint GetUboId(const GLchar* param) const { return glGetUniformBlockIndex(ProgramId, param); }
	const GLuint GetUBO_BP() const { return UBO_BP; }

	virtual void Use();

	void Destroy();
};