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
	Vec3 lightPosition, skyColor;
	std::shared_ptr<Camera> camera;
	bool fog;

public:

	TextureShader();

	void SetTexture(const char* path);
	void SetNormalTexture(const char * path);
	void SetCamera(std::shared_ptr<Camera> camera);
	void SetTexture(unsigned int textureId) { this->textureId = textureId; }
	void SetLightPosition(Vec3 pos);
	void setSkyColor(Vec3 colour);
	void SetFog(bool fog) { this->fog = fog; }
	//void setTextureToFrameBuffer(unsigned int renderTexture);
	void Use();
};