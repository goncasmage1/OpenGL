#include "TextureShader.h"
#include "SOIL.h"

TextureShader::TextureShader()
{
	std::vector<ShaderAttribute> shaderAttributes = {
		ShaderAttribute(0, "in_Position"),
		ShaderAttribute(1, "in_Coordinates"),
		ShaderAttribute(2, "in_Normal")
	};
	std::vector<std::string> shaderPaths = {
		"src/Shader/GLSL/TextureVertexShader.glsl",
		"src/Shader/GLSL/TextureFragmentShader.glsl"
	};

	BuildShader(shaderAttributes, shaderPaths);

}


void TextureShader::SetTexture(const char* path)
{

	glGenTextures(1, &textureId);
	
	int width, height;

	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );	// Use GL_MIRRORED_REPEAT to prevent white borders. Due to interpolation it takes value from next repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
}


void TextureShader::SetNormalTexture(const char* path) {

	glGenTextures(1, &normalTextureId);

	int width, height;

	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, normalTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Use GL_MIRRORED_REPEAT to prevent white borders. Due to interpolation it takes value from next repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
}

/*void TextureShader::setTextureToFrameBuffer(unsigned int renderTexture)
{
	this->textureId = renderTexture;
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
	//glDrawBuffers(1, GL_COLOR_ATTACHMENT0);
	
}*/

void TextureShader::SetCamera(std::shared_ptr<Camera> camera)
{
	this->camera = camera;
}

void TextureShader::SetCameraPos()
{
	Vec3 direction = camera->GetDirection();
	CameraPositionX = direction.x;
	CameraPositionY = direction.y;
	CameraPositionZ = direction.z;
}

void TextureShader::SetLightPosition(Vec3 pos)
{
	this->lightPosition = pos;
}

void TextureShader::SetLightColour(Vec3 colour)
{
	this->lightColour = colour;
}

void TextureShader::Use()
{
	ShaderProgram::Use();

	if (textureId != 0) {
		glUniform1i(glGetUniformLocation(ProgramId, "screenTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	if (normalTextureId != 0) {
		glUniform1i(glGetUniformLocation(ProgramId, "normalTexture"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalTextureId);

		glUniform1i(glGetUniformLocation(ProgramId, "normalMapping"), true);
	}

	glUniform3f(glGetUniformLocation(ProgramId, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);

	SetCameraPos();
	glUniform3f(glGetUniformLocation(ProgramId, "viewPos"), CameraPositionX, CameraPositionY, CameraPositionZ);
	//glUniform4f(glGetUniformLocation(ProgramId, "plane"), plane.x, plane.y, plane.z, plane.w);
}
