#pragma once

#include <memory>
#include "ShaderProgram.h"
#include "..\Math\Vector.h"
#include "..\Camera.h"

class TextureShader : public ShaderProgram
{
private:
	GLuint textureId = 0;
	GLuint normalTextureId = 0;
	Vec4 plane;
	Vec3 lightPosition;
	Vec3 lightColour;
	int CameraPositionX, CameraPositionY, CameraPositionZ;
	std::shared_ptr<Camera> camera;

public:

	TextureShader();

	void SetTexture(const char* path);
	void SetNormalTexture(const char * path);
	void SetCamera(std::shared_ptr<Camera> camera);
	void SetCameraPos();
	void SetTexture(unsigned int textureId) { this->textureId = textureId; }
	void SetLightPosition(Vec3 pos);
	void SetLightColour(Vec3 colour);
	//void setTextureToFrameBuffer(unsigned int renderTexture);
	void Use();
};