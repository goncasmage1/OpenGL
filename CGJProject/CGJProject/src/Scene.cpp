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
	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), camera->GetViewMatrix().GetData());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mat4), sizeof(Mat4), camera->GetProjectionMatrix(true).GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for (std::shared_ptr<SceneNode> node : root->GetChildren())
	{
		node->Draw();
	}
}


