#include "SkyboxShader.h"
#include <cassert>

SkyboxShader::SkyboxShader(Mat4 view)
{
	std::vector<ShaderAttribute> shaderAttributes = {
	ShaderAttribute(0, "in_Position")
	};
	std::vector<std::string> shaderPaths = {
		"src/Shader/GLSL/SkyboxVertexShader.glsl",
		"src/Shader/GLSL/SkyboxFragmentShader.glsl"
	};
	SetViewMatrix(view);
	BuildShader(shaderAttributes, shaderPaths);
	LoadCubeMap(faces);
}

GLuint SkyboxShader::GetTexture()
{
	return textureId;
}

void SkyboxShader::SetViewMatrix(Mat4 view)
{
	this->view = Mat4(Mat3(view));
}

void SkyboxShader::LoadCubeMap(std::vector<const char*> faces)
{
	assert(faces.size() == 6);

	unsigned char* image;
	int width, height;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	for (GLuint i = 0; i < faces.size(); i++)
	{
		std::cout << faces[i] << std::endl;

		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyboxShader::Use()
{
	glDepthMask(GL_FALSE);
	ShaderProgram::Use();
	
	if (textureId != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(ProgramId, "skybox"), 0);
		glUniformMatrix4fv(glGetUniformLocation(ProgramId, "view"), 1, GL_FALSE, view.GetData());
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	}
}

