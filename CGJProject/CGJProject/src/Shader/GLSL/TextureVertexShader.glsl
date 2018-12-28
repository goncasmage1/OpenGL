#version 330 core

in vec4 in_Position;
in vec2 in_Coordinates;
in vec3 in_Normal;
//out vec4 ex_Color;
out vec2 ex_textCoord;

uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform vec4 plane;

void main(void)
{
	vec4 worldPosition = ModelMatrix * in_Position;
	gl_Position = ProjectionMatrix * ViewMatrix * worldPosition;
	//gl_ClipDistance[0] = dot(worldPosition, plane);
	ex_textCoord = in_Coordinates;
}