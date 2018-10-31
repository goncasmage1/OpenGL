#include "ShaderProgram.h"

#include "Shader.h"
#include "VertexShader.h"
#include "FragmentShader.h"

#define VERTICES 0
#define COLORS 1

ShaderProgram::ShaderProgram(std::vector<ShaderAttribute> Attributes)
{
	VertexShader VShader = VertexShader();
	FragmentShader FShader = FragmentShader();

	ProgramId = glCreateProgram();
	VShader.Attach(ProgramId);
	FShader.Attach(ProgramId);

	for (ShaderAttribute attr : Attributes) glBindAttribLocation(ProgramId, attr.index, attr.name);

	glLinkProgram(ProgramId);
	CheckLinkage();
	glUniformBlockBinding(ProgramId, GetUboId("SharedMatrices"), UBO_BP);

	VShader.DetachAndDelete(ProgramId);
	FShader.DetachAndDelete(ProgramId);
}

void ShaderProgram::CheckLinkage()
{
	GLint linked;
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &length);
		GLchar* const log = new char[length];
		glGetProgramInfoLog(ProgramId, length, &length, log);
		std::cerr << "[LINK] " << std::endl << log << std::endl;
		delete log;
	}
}

void ShaderProgram::Destroy()
{
	glUseProgram(0);
	glDeleteProgram(ProgramId);
}
