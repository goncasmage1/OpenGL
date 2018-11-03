#include "Shader.h"
#include <fstream>

Shader::Shader(const std::string& shaderPath, const GLenum shader_type)
{
	std::ifstream myReadFile(shaderPath);
	std::string content((std::istreambuf_iterator<char>(myReadFile)),
		(std::istreambuf_iterator<char>()));

	const GLchar* shader = content.c_str();

	id = glCreateShader(shader_type);

	glShaderSource(id, 1, &shader, 0);
	glCompileShader(id);
	CheckCompilation(id, shaderPath);
}

const GLuint Shader::CheckCompilation(const GLuint shader_id, const std::string& filename)
{
	GLint compiled;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
		GLchar* const log = new char[length];
		glGetShaderInfoLog(shader_id, length, &length, log);
		std::cerr << "[" << filename << "] " << std::endl << log;
		delete log;
	}
	return compiled;
}

void Shader::Attach(GLuint ProgramId)
{
	glAttachShader(ProgramId, id);
}

void Shader::DetachAndDelete(GLuint ProgramId)
{
	glDetachShader(ProgramId, id);
	glDeleteShader(id);
}
