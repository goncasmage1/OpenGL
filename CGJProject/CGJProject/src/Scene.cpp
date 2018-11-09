#include "Camera.h"
#include "Mesh.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Shader/ShaderProgram.h"
#include "Math/Matrix.h"

Scene::Scene(std::shared_ptr<Camera> newCamera, std::shared_ptr<ShaderProgram> newShaderProgram) :
	camera(newCamera), shaderProg(newShaderProgram), root(std::make_shared<SceneNode>(nullptr, nullptr, newShaderProgram))
{
}

void Scene::Init()
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
	camera->Draw();

	for (std::shared_ptr<SceneNode> node : root->GetChildren())
	{
		node->Draw();
	}
}


