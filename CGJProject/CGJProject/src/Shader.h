#pragma once
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

class Shader
{
protected:

	GLuint id;

public:

	virtual void Attach(GLuint ProgramId);
	virtual void DetachAndDelete(GLuint ProgramId);
	GLuint GetID() const { return id; }
};