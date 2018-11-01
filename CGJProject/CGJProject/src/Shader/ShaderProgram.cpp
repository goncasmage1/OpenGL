#include "ShaderProgram.h"

#include "Shader.h"

#define VERTICES 0
#define COLORS 1

ShaderProgram::ShaderProgram(const std::vector<ShaderAttribute>& Attributes, const std::vector<std::string>& ShaderPaths)
{
	size_t NumOfShaders = ShaderPaths.size();
	std::vector<Shader> shaders;
	int i = 0;

	for (i = 0; i < NumOfShaders; i++) shaders.push_back(Shader(ShaderPaths[i]));

	ProgramId = glCreateProgram();
	for (i = 0; i < NumOfShaders; i++) shaders[i].Attach(ProgramId);

	for (ShaderAttribute attr : Attributes) glBindAttribLocation(ProgramId, attr.index, attr.name);

	glLinkProgram(ProgramId);
	CheckLinkage();
	glUniformBlockBinding(ProgramId, GetUboId("SharedMatrices"), UBO_BP);

	for (i = 0; i < NumOfShaders; i++) shaders[i].DetachAndDelete(ProgramId);
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
