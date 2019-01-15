#version 330 core

in vec4 in_Position;
out vec3 textureDir;

uniform mat4 ModelMatrix;
uniform mat4 view;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	
	gl_Position = ProjectionMatrix * view * in_Position;
	textureDir = vec3(in_Position);
}