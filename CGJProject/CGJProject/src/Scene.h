#pragma once

#include <iostream>

class Scene
{
private:

	GLuint VaoId, VboId[2];

	std::shared_ptr<class SceneNode> root = nullptr;
	std::shared_ptr<class Camera> camera = nullptr;

public:

	Scene();
	Scene(std::shared_ptr<class Camera> newCamera);

	void CreateBufferObjects();
	void DestroyBufferObjects();

	std::shared_ptr<class SceneNode> GetRoot() { return root; }

	void Draw();
};

