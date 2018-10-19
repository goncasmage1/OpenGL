#pragma once
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

class Shader
{
protected:

	GLuint id;

public:

	void Attach(GLuint ProgramId);
	void DetachAndDelete(GLuint ProgramId);
	GLuint GetID() const { return id; }
};