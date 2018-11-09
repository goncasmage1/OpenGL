#include "Camera.h"
#include "Mesh.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Math/Matrix.h"

Scene::Scene() :
	root(std::make_shared<SceneNode>(nullptr, nullptr))
{
}

Scene::Scene(std::shared_ptr<class Camera> newCamera) :
	root(std::make_shared<SceneNode>(nullptr, nullptr)), camera(newCamera)
{
}

void Scene::CreateBufferObjects()
{
}

void Scene::DestroyBufferObjects()
{
}

void Scene::Draw()
{
	for (std::shared_ptr<SceneNode> node : root->GetChildren())
	{
		node->Draw(camera->GetViewMatrix().GetData(), camera->GetProjectionMatrix(true).GetData());
	}
}


