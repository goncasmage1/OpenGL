#pragma once

#include "../Camera.h"
#include "../WaterFrameBuffer.h"
#include "ShaderProgram.h"



class WaterShader : public ShaderProgram
{
private:
	const char* dudvPath = "../../assets/Textures/Ripple/waterDUDV.png";
	const char* normalPath = "../../assets/Textures/Ripple/normalMap.png";
	const float waveSpeed = 0.03f;

	float moveFactor;
	std::shared_ptr<WaterFrameBuffer> fbo;
	int reflectionTextureId;
	int refractionTextureId;
	int dudvTexture;
	int normalTexture;
	Vec3 lightPosition;
	Vec3 lightColour;
	
	int CameraPositionX, CameraPositionY, CameraPositionZ;
	std::shared_ptr<Camera> camera; 

public:

	WaterShader();
	int loadTexture(const char* path);
	
	void SetFBO(std::shared_ptr<WaterFrameBuffer> fbo) { this->fbo = fbo; }
	void SetCamera(std::shared_ptr<Camera> camera);
	void SetCameraPos(); 
	void SetLightPosition(Vec3 pos);
	Vec3 GetLightPosition() { return this->lightPosition;  }
	void SetLightColour(Vec3 colour);
	void Use();
};