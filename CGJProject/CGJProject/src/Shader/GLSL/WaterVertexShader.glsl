#version 330 core

in vec4 in_Position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;


uniform vec3 cameraPosition;
uniform vec3 lightPosition;

uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

const float tilling = 1.0f;

void main(void)
{
	vec4 worldPosition = ModelMatrix * in_Position;
	clipSpace = ProjectionMatrix *  ViewMatrix * worldPosition;
	
	gl_Position = clipSpace;
	textureCoords = vec2(in_Position.x/2.0 + 0.5, in_Position.z/2.0 + 0.5)*tilling;
	toCameraVector = cameraPosition - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - lightPosition;
}