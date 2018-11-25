#include "WoodShader.h"

WoodShader::WoodShader()
{
	std::vector<ShaderAttribute> shaderAttributes = {
		ShaderAttribute(0, "in_Position"),
		ShaderAttribute(1, "in_Coordinates"),
		ShaderAttribute(2, "in_Normal")
	};
	std::vector<std::string> shaderPaths = {
		"src/Shader/GLSL/WoodVertexShader.glsl",
		"src/Shader/GLSL/WoodFragmentShader.glsl"
	};

	BuildShader(shaderAttributes, shaderPaths);
}
