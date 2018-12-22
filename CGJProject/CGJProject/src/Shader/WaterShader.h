#pragma once

#include "../Camera.h"
#include "../WaterFrameBuffer.h"
#include "ShaderProgram.h"


class WaterShader : public ShaderProgram
{
private:
	std::shared_ptr<WaterFrameBuffer> fbo;
	int reflectionTextureId;
	int refractionTextureId;

	GLuint skyboxTextureId = 0; //remove me
	int CameraPositionX, CameraPositionY, CameraPositionZ;
	std::shared_ptr<Camera> camera; 

public:

	WaterShader();
	void SetFBO(std::shared_ptr<WaterFrameBuffer> fbo) { this->fbo = fbo; }
	void SetCamera(std::shared_ptr<Camera> camera);
	void SetCameraPos(); 
	void SetSkyboxTexture(GLuint skyboxTexture); //remove me
	void Use();
	
};