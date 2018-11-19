#include "Camera.h"
#include "Mesh.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Shader/ShaderProgram.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

Scene::Scene(std::shared_ptr<Camera> newCamera, std::shared_ptr<ShaderProgram> newShaderProgram) :
	camera(newCamera), root(std::make_shared<SceneNode>(nullptr, Transform(), nullptr, newShaderProgram, Vec3()))
{
}

void Scene::Draw()
{
	for (std::shared_ptr<SceneNode> node : root->childNodes)
	{
		node->Draw();
	}
}


