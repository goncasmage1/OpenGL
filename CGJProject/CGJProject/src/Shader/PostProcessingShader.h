#pragma once

#include "ShaderProgram.h"

class PostProcessingShader : public ShaderProgram {

protected:

	GLuint v_coord, fbo_texture, uniform_fbo_texture;

	virtual void BuildShader(const std::vector<ShaderAttribute> Attributes, const std::vector<std::string> ShaderPaths) override;

public:

	PostProcessingShader();

	virtual void Use() override;

	void SetFboTexture(GLuint new_fbo_texture);

	GLuint GetVCoordId() const { return v_coord; }

};