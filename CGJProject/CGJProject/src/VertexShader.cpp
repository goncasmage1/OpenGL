#include "VertexShader.h"
#include <string>
#include <fstream>

VertexShader::VertexShader()
{
	std::ifstream myReadFile("src/VertexShader.vert");
	std::string content((std::istreambuf_iterator<char>(myReadFile)),
		(std::istreambuf_iterator<char>()));

	const GLchar* shader = content.c_str();

	id = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(id, 1, &shader, 0);
	glCompileShader(id);
}

