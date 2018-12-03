#include "SailShader.h"

SailShader::SailShader()
{
	std::vector<ShaderAttribute> shaderAttributes = {
		ShaderAttribute(0, "in_Position"),
		ShaderAttribute(1, "in_Coordinates"),
		ShaderAttribute(2, "in_Normal")
	};
	std::vector<std::string> shaderPaths = {
		"src/Shader/GLSL/SailVertexShader.glsl",
		"src/Shader/GLSL/SailFragmentShader.glsl"
	};

	BuildShader(shaderAttributes, shaderPaths);
}

