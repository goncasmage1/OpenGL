#pragma once

#include <iostream>

class Scene
{
private:

	std::shared_ptr<class Camera> camera = nullptr;
	std::shared_ptr<class ShaderProgram> shaderProg = nullptr;

public:

	Scene(std::shared_ptr<class Camera> newCamera, std::shared_ptr<class ShaderProgram> newShaderProgram);

	std::shared_ptr<class SceneNode> root = nullptr;

	void Draw();
};

