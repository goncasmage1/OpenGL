#version 330 core

in vec4 in_Position;
in vec2 in_Coordinates;
in vec3 in_Normal;
out vec4 ex_Color;

uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
	ex_Color = vec4(1.0, 1.0, 1.0, 1.0);
}