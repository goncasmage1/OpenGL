#version 330 core

in vec4 in_Position;
in vec2 in_Coordinates;
in vec3 in_Normal;

out VS_OUT {
	vec3 FragmentPosition;
	vec3 Normal;
	vec2 TextCoords;
} vs_out;

uniform vec4 plane;
uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
    vs_out.FragmentPosition = in_Position.xyz;
    vs_out.Normal = in_Normal;
    vs_out.TextCoords = in_Coordinates;
	vec4 WorldPosition = ModelMatrix * in_Position;
	gl_Position = ProjectionMatrix * ViewMatrix * WorldPosition;
	gl_ClipDistance[0] = dot(WorldPosition, plane);	
}