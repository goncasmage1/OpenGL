#pragma once

#include <iostream>
#include "Math/Vector.h"
#include <memory>

class Scene
{
private:

	std::shared_ptr<class Camera> camera = nullptr;

public:

	Scene(std::shared_ptr<class Camera> newCamera);

	std::shared_ptr<class SceneNode> root = nullptr;

	void Draw(Vec4 plane);
};

