#include "FragmentShader.h"

#include <fstream>

FragmentShader::FragmentShader()
{
	std::ifstream myReadFile("src/Shader/FragmentShader.frag");
	std::string content((std::istreambuf_iterator<char>(myReadFile)),
		(std::istreambuf_iterator<char>()));

	const GLchar* shader = content.c_str();

	id = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(id, 1, &shader, 0);
	glCompileShader(id);
}