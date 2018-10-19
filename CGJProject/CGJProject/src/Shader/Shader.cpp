#include "Shader.h"

void Shader::Attach(GLuint ProgramId)
{
	glAttachShader(ProgramId, id);
}

void Shader::DetachAndDelete(GLuint ProgramId)
{
	glDetachShader(ProgramId, id);
	glDeleteShader(id);
}
