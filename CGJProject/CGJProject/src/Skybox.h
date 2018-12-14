#pragma once
#include <map>
#include <memory>
#include "Texture.h"
#include "Mesh.h"
#include "Shader/ShaderProgram.h"
#include "Math/Matrix.h"

class Skybox
{
public:
	Mat4 transformationMatrix;

	std::vector<std::shared_ptr<Texture>> textures;
	std::shared_ptr<Mesh> cube;
	std::shared_ptr<class ShaderProgram> shaderProg = nullptr;

	GLuint TextureId;

	Skybox();
	void Use();
	void Draw();
	~Skybox();
};

