#version 330 core

layout (location = 0) in vec4 position;

out vec3 TexCoords;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main()
{
	gl_Position = ProjectionMatrix * mat4(mat3(ViewMatrix)) * position;
	TexCoords = vec3(position);
}