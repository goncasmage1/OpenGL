#pragma once

#include <iostream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

class Shader
{
protected:

	const GLuint CheckCompilation(const GLuint shader_id, const std::string& filename);

	GLuint id;

public:

	Shader(const std::string& shaderPath);

	void Attach(GLuint ProgramId);
	void DetachAndDelete(GLuint ProgramId);
	GLuint GetID() const { return id; }
};