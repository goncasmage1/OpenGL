#include "TextureShader.h"
#include "SOIL.h"

TextureShader::TextureShader()
{
	std::vector<ShaderAttribute> shaderAttributes = {
		ShaderAttribute(0, "in_Position"),
		ShaderAttribute(1, "in_Coordinates"),
		ShaderAttribute(2, "in_Normal"),
		ShaderAttribute(3, "tangent"),
		ShaderAttribute(4, "bitangent")
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

void TextureShader::setSkyColor(Vec3 color) {
	this->skyColor = color;
}

void TextureShader::SetCamera(std::shared_ptr<Camera> camera)
{
	this->camera = camera;
}

void TextureShader::SetLightPosition(Vec3 pos)
{
	this->lightPosition = pos;
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

	glUniform3f(glGetUniformLocation(ProgramId, "lightPos"), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(glGetUniformLocation(ProgramId, "viewPos"), camera->GetOffset().x, camera->GetOffset().y, camera->GetOffset().z);

	//Fog Stuff and Things
	glUniform3f(glGetUniformLocation(ProgramId, "skyColour"), this->skyColor.x, this->skyColor.y, this->skyColor.z);
}
