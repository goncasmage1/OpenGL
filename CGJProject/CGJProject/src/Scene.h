#pragma once

#include <iostream>

class Scene
{

	std::shared_ptr<class SceneNode> root = nullptr;
	std::shared_ptr<class Camera> camera = nullptr;

	void CreateBufferObjects();
	void DestroyBufferObjects();

public:

	Scene();
	Scene(std::shared_ptr<class Camera> newCamera);

	std::shared_ptr<class SceneNode> GetRoot() { return root; }

	void Draw();
};

