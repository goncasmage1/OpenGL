#include "ShaderProgram.h"

#include "VertexShader.h"
#include "FragmentShader.h"

ShaderProgram::ShaderProgram()
{
	VertexShader VShader = VertexShader{};
	FragmentShader FShader = FragmentShader();

	ProgramId = glCreateProgram();
	VShader.Attach(ProgramId);
	FShader.Attach(ProgramId);

	glLinkProgram(ProgramId);

	VShader.DetachAndDelete(ProgramId);
	FShader.DetachAndDelete(ProgramId);
}

void ShaderProgram::Destroy()
{
	glUseProgram(0);
	glDeleteProgram(ProgramId);
}
