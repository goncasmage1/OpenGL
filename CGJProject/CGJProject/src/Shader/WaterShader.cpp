#include "WaterShader.h"
#include "SOIL.h"

WaterShader::WaterShader()
{
	dudvTexture = loadTexture(dudvPath);
	normalTexture = loadTexture(normalPath);
	moveFactor = 0.0f;

	std::vector<ShaderAttribute> shaderAttributes = {
	ShaderAttribute(0, "in_Position")
	};
	std::vector<std::string> shaderPaths = {
		"src/Shader/GLSL/WaterVertexShader.glsl",
		"src/Shader/GLSL/WaterFragmentShader.glsl"
	};

	BuildShader(shaderAttributes, shaderPaths);
}

int WaterShader::loadTexture(const char* path)
{
	GLuint textureId;
	int width, height;
	GLboolean alpha = false;

	glGenTextures(1, &textureId);
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Use GL_MIRRORED_REPEAT to prevent white borders. Due to interpolation it takes value from next repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	
	return textureId;
}

void WaterShader::SetCamera(std::shared_ptr<Camera> camera)
{
	this->camera = camera;
}

void WaterShader::SetCameraPos()
{
	Vec3 direction = camera->GetDirection();
	CameraPositionX = direction.x;
	CameraPositionY = direction.y;
	CameraPositionZ = direction.z;
}

void WaterShader::SetLightPosition(Vec3 pos)
{
	this->lightPosition = pos;
}

void WaterShader::SetLightColour(Vec3 colour)
{
	this->lightColour = colour;
}

void WaterShader::Use()
{
	ShaderProgram::Use();

	glUniform1i(glGetUniformLocation(ProgramId, "reflectionTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->getReflectionTexture());	
	
	glUniform1i(glGetUniformLocation(ProgramId, "refractionTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbo->getRefractionTexture());

	glUniform1i(glGetUniformLocation(ProgramId, "dudvMap"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);

	glUniform1i(glGetUniformLocation(ProgramId, "normalMap"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalTexture);

	glUniform1i(glGetUniformLocation(ProgramId, "depthMap"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, fbo->getRefractionDepthTexture());

	moveFactor += waveSpeed * 0.008;
	moveFactor = fmod(moveFactor, 1);
	glUniform1f(glGetUniformLocation(ProgramId, "moveFactor"), moveFactor);

	glUniform3f(glGetUniformLocation(ProgramId, "cameraOffset"), camera->GetCameraMovement().x, camera->GetCameraMovement().y, camera->GetCameraMovement().z);
	
	// LIGHT //
	glUniform3f(glGetUniformLocation(ProgramId, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(glGetUniformLocation(ProgramId, "lightColour"), lightColour.x, lightColour.y, lightColour.z);
	//////////

	SetCameraPos();
	glUniform3f(glGetUniformLocation(ProgramId, "cameraPos"), CameraPositionX, CameraPositionY, CameraPositionZ);
	
	
}
