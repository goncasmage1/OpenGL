#include "ShaderProgram.h"

#include "Shader.h"
#include "VertexShader.h"
#include "FragmentShader.h"

#define VERTICES 0
#define COLORS 1

ShaderProgram::ShaderProgram(std::vector<ShaderAttribute> Attributes)
{
	VertexShader VShader = VertexShader{};
	FragmentShader FShader = FragmentShader();

	ProgramId = glCreateProgram();
	VShader.Attach(ProgramId);
	FShader.Attach(ProgramId);

	for (ShaderAttribute attr : Attributes) glBindAttribLocation(ProgramId, attr.index, attr.name);

	glLinkProgram(ProgramId);
	glUniformBlockBinding(ProgramId, GetUboId("SharedMatrices"), UBO_BP);

	VShader.DetachAndDelete(ProgramId);
	FShader.DetachAndDelete(ProgramId);
}

void ShaderProgram::Destroy()
{
	glUseProgram(0);
	glDeleteProgram(ProgramId);
}
