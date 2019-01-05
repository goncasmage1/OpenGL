#include "RTT.h"

RTT::RTT()
{

	std::vector<ShaderAttribute> shaderAttributes = {
		ShaderAttribute(0, "in_Position"),
		ShaderAttribute(1, "in_Coordinates"),
		ShaderAttribute(2, "in_Normal")
	};
	std::vector<std::string> shaderPaths = {
		"src/Shader/GLSL/RTTVertexShader.glsl",
		"src/Shader/GLSL/RTTFragmentShader.glsl"
	};

	BuildShader(shaderAttributes, shaderPaths);


}

void RTT::Use()
{
	ShaderProgram::Use();

	if (textureId != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
}
