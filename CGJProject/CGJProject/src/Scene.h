#pragma once

#include <iostream>

class Scene
{

	std::shared_ptr<class SceneNode> root;

public:

	Scene();

	std::shared_ptr<class SceneNode> GetRoot() { return root; }

	void Draw();
};

