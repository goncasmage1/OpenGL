#include "Skybox.h"

Skybox::Skybox()
{

}

Skybox::Skybox(std::vector<std::string> filePaths, std::shared_ptr<Mesh> cube, std::shared_ptr<ShaderProgram> shaderProg, Mat4 matrix) {

	this->cube = cube;
	this->shaderProg = shaderProg;
	this->transformationMatrix = matrix;

	for (int i = 0; i < filePaths.size(); i++) {
		textures.push_back(std::make_shared<Texture>(Texture(filePaths[i])));
	}

	glGenTextures(1, &TextureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureId);

	for (GLint i = 0; i < textures.size(); i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textures[i]->WIDTH, textures[i]->HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, textures[i]->image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::Draw() {
	glDepthMask(GL_FALSE);
	shaderProg->Use();
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureId);
	cube->Draw();
	glDepthMask(GL_TRUE);
}

Skybox::~Skybox()
{
}
