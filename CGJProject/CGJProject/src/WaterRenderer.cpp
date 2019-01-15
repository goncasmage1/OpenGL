#include "WaterRenderer.h"
#include "Shader/ShaderProgram.h"

void WaterRenderer::Draw(Vec4 plane)
{
	if (shaderProg != nullptr && mesh != nullptr)
	{
		shaderProg->SetPlane(plane);
		shaderProg->Use();
		SetupUniforms();
		mesh->Draw();
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glUseProgram(0);
	}
}
