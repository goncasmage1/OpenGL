#version 330 core

in vec4 in_Position;
in vec2 in_Coordinates;
in vec3 in_Normal;
//out vec4 ex_Color;
out vec3 textureDir;

uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * in_Position;
	textureDir = vec3(in_Position);
}