#include "WaterShader.h"

WaterShader::WaterShader()
{
	std::vector<ShaderAttribute> shaderAttributes = {
	ShaderAttribute(0, "in_Position"),
	ShaderAttribute(1, "in_Coordinates"),
	ShaderAttribute(2, "in_Normal")
	};
	std::vector<std::string> shaderPaths = {
		"src/Shader/GLSL/WaterVertexShader.glsl",
		"src/Shader/GLSL/WaterFragmentShader.glsl"
	};

	BuildShader(shaderAttributes, shaderPaths);
}

void WaterShader::SetCamera(std::shared_ptr<Camera> camera)
{
	this->camera = camera;
}

void WaterShader::SetCameraPos()
{
	/*Mat4 model = camera->GetModelMat();
	float * data = model.GetData();

	CameraPositionX = data[12];
	CameraPositionY = data[13];
	CameraPositionZ = data[14];*/

	Vec3 direction = camera->GetDirection();
	CameraPositionX = direction.x;
	CameraPositionY = direction.y;
	CameraPositionZ = direction.z;
}

void WaterShader::SetSkyboxTexture(GLuint skyboxTexture)
{
	skyboxTextureId = skyboxTexture;
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

	SetCameraPos();
	glUniform3f(glGetUniformLocation(ProgramId, "cameraPos"), CameraPositionX, CameraPositionY, CameraPositionZ);
}
