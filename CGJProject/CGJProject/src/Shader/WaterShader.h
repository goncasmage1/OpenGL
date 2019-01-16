#pragma once

#include <memory>
#include "../Camera.h"
#include "../WaterFrameBuffer.h"
#include "ShaderProgram.h"

class WaterShader : public ShaderProgram
{
private:
	const char* dudvPath = "../../assets/Textures/Ripple/waterDUDV.png";
	const char* normalPath = "../../assets/Textures/Ripple/normalMap.png"; //this one looks better
	//const char* normalPath = "../../assets/Textures/Ripple/mNormalMap.png";
	const float waveSpeed = 0.03f;


	std::shared_ptr<WaterFrameBuffer> fbo;
	std::shared_ptr<Camera> camera;
	int reflectionTextureId, refractionTextureId, dudvTexture, normalTexture;
	float moveFactor;
	Vec3 Position, lightPosition, lightColour; 

public:

	WaterShader(Vec3 pos, Vec3 lightPos, Vec3 lightColor);
	int loadTexture(const char* path);
	
	void SetFBO(std::shared_ptr<WaterFrameBuffer> fbo) { this->fbo = fbo; }
	void SetCamera(std::shared_ptr<Camera> camera);
	void Use();

	Vec3 GetPosition() { return Position; }
};