#include "PostProcessingShader.h"
#include "Shader.h"

PostProcessingShader::PostProcessingShader()
{
	std::vector<ShaderAttribute> shaderAttributes = {
		ShaderAttribute(0, "v_coord")
	};
	std::vector<std::string> shaderPaths = {
		"src/Shader/GLSL/PostProcessingVertexShader.glsl",
		"src/Shader/GLSL/PostProcessingFragmentShader.glsl"
	};

	BuildShader(shaderAttributes, shaderPaths);
}

void PostProcessingShader::BuildShader(const std::vector<ShaderAttribute> Attributes, const std::vector<std::string> ShaderPaths)
{
	size_t NumOfShaders = ShaderPaths.size();
	std::vector<Shader> shaders;
	std::vector<GLenum> shaderTypes = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

	int i = 0;
	for (i = 0; i < NumOfShaders; i++) shaders.push_back(Shader(ShaderPaths[i], shaderTypes[i]));

	ProgramId = glCreateProgram();
	for (i = 0; i < NumOfShaders; i++) shaders[i].Attach(ProgramId);

	for (ShaderAttribute attr : Attributes) glBindAttribLocation(ProgramId, attr.index, attr.name);

	glLinkProgram(ProgramId);
	CheckLinkage();

	for (i = 0; i < NumOfShaders; i++) shaders[i].DetachAndDelete(ProgramId);

	std::string attribute_name;
	attribute_name = "v_coord";
	v_coord = GetAttributeId(attribute_name.c_str());
	if (v_coord == -1) {
		std::cerr << "Could not bind " << attribute_name << " to Post Processing Shader!" << std::endl;
	}

	attribute_name = "fbo_texture";
	uniform_fbo_texture = GetUniformId(attribute_name.c_str());
	if (uniform_fbo_texture == -1) {
		std::cerr << "Could not bind " << attribute_name << " to Post Processing Shader!" << std::endl;
	}
}

void PostProcessingShader::SetFboTexture(GLuint new_fbo_texture)
{
	fbo_texture = new_fbo_texture;
}

void PostProcessingShader::Use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ProgramId);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(uniform_fbo_texture, /*GL_TEXTURE*/0);
}
