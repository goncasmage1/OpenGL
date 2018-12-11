#include "ShaderProgram.h"

#include "Shader.h"

#define VERTICES 0
#define COLORS 1

///////////////////////
//	ShaderAttribute
///////////////////////

ShaderAttribute::ShaderAttribute(int newIndex, const GLchar * newName) :
	index(newIndex), name(newName)
{
}

///////////////////////
//	ShaderProgram
///////////////////////

ShaderProgram::ShaderProgram()
{
	std::vector<ShaderAttribute> shaderAttributes = {
		ShaderAttribute(0, "in_Position"),
		ShaderAttribute(1, "in_Coordinates"),
		ShaderAttribute(2, "in_Normal")
	};
	std::vector<std::string> shaderPaths = {
		"src/Shader/GLSL/WhiteShader.glsl",
		"src/Shader/GLSL/FragmentShader.glsl"
	};

	BuildShader(shaderAttributes, shaderPaths);
}

ShaderProgram::ShaderProgram(const std::vector<ShaderAttribute> Attributes, const std::vector<std::string> ShaderPaths)
{
	BuildShader(Attributes, ShaderPaths);
}

void ShaderProgram::BuildShader(const std::vector<ShaderAttribute> Attributes, const std::vector<std::string> ShaderPaths)
{
	size_t NumOfShaders = ShaderPaths.size();
	std::vector<Shader> shaders;
	std::vector<GLenum> shaderTypes = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

	int i = 0;
	for (i = 0; i < NumOfShaders; i++) shaders.push_back(Shader(ShaderPaths[i], shaderTypes[i]));

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

void ShaderProgram::Use()
{
	glUseProgram(ProgramId);
}

void ShaderProgram::Destroy()
{
	glUseProgram(0);
	glDeleteProgram(ProgramId);
}