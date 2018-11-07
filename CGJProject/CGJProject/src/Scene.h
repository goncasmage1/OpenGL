#pragma once

#include <iostream>

class Scene
{

	std::shared_ptr<class SceneNode> root;

	void CreateBufferObjects();
	void DestroyBufferObjects();

public:

	Scene();

	std::shared_ptr<class SceneNode> GetRoot() { return root; }

	void Draw();
};

