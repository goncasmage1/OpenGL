#include "Scene.h"
#include "SceneNode.h"
#include "Mesh.h"
#include "Math/Matrix.h"

Scene::Scene()
{
	root = std::make_shared<SceneNode>(nullptr);
}

void Scene::Draw()
{
	for (std::shared_ptr<SceneNode> node : root->GetChildren())
	{

	}
}


