#pragma once

#include <memory>
#include <vector>
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
	TextureShader(const std::vector<ShaderAttribute> Attributes, const std::vector<std::string> ShaderPaths);

	void SetTexture(const char* path);
	void SetNormalTexture(const char * path);
	void SetCamera(std::shared_ptr<Camera> camera);
	void SetTexture(unsigned int textureId) { this->textureId = textureId; }
	void SetLightPosition(Vec3 pos);
	void setSkyColor(Vec3 colour);
	void SetFog(bool fog) { this->fog = fog; }
	void Use();
};