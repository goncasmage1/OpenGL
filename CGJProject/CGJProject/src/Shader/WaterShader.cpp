#include "WaterShader.h"

WaterShader::WaterShader()
{
	std::vector<ShaderAttribute> shaderAttributes = {
		ShaderAttribute(0, "in_Position"),
		ShaderAttribute(1, "in_Coordinates"),
		ShaderAttribute(2, "in_Normal")
	};
	std::vector<std::string> shaderPaths = {
		"src/Shader/GLSL/WaterVertexShader.glsl",
		"src/Shader/GLSL/WaterFragmentShader.glsl"
	};

	BuildShader(shaderAttributes, shaderPaths);
}
