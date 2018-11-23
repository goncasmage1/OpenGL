#include "Camera.h"
#include "Mesh.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Shader/ShaderProgram.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

Scene::Scene(std::shared_ptr<Camera> newCamera) :
	camera(newCamera), root(std::make_shared<SceneNode>(nullptr, Transform(), nullptr, nullptr))
{
}

void Scene::Draw()
{
	for (std::shared_ptr<SceneNode> node : root->childNodes)
	{
		node->Draw();
	}
}


