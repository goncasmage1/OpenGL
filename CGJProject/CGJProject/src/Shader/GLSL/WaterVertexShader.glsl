#version 330 core

in vec4 in_Position;
in vec2 in_Coordinates;
in vec3 in_Normal;

out vec4 clipSpace;
out vec3 toCameraVector;

uniform vec3 cameraPos;

uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};


void main(void)
{
	vec4 worldPosition =   ModelMatrix * vec4(in_Position.x, 0.0, in_Position.z, 1.0);
	//vec4 worldPosition =   ModelMatrix * in_Position;
	clipSpace = ProjectionMatrix *  ViewMatrix * worldPosition;
	gl_Position = clipSpace;
	toCameraVector = cameraPos - worldPosition.xyz;
}