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

/*void TextureShader::setTextureToFrameBuffer(unsigned int renderTexture)
{
	this->textureId = renderTexture;
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
	//glDrawBuffers(1, GL_COLOR_ATTACHMENT0);
	
}*/

void TextureShader::Use()
{
	ShaderProgram::Use();

	if (textureId != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
	//glUniform4f(glGetUniformLocation(ProgramId, "plane"), plane.x, plane.y, plane.z, plane.w);
}
