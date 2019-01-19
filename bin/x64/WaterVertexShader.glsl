#version 330 core

in vec4 in_Position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;
out float visibility;


uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform bool fog;

uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

const float tilling = 2.0f;
const float density = 0.03;
const float gradient = 1.5;

void main(void)
{
	vec4 worldPosition = ModelMatrix * in_Position;
	vec4 positionRelativeCamera = ViewMatrix * worldPosition;
	clipSpace = ProjectionMatrix * positionRelativeCamera;
	
	gl_Position = clipSpace;
	textureCoords = vec2(in_Position.x/2.0 + 0.5, in_Position.z/2.0 + 0.5)*tilling;
	toCameraVector = cameraPosition - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - lightPosition;

	if (fog)
	{
		//Fog
		float distance = length(positionRelativeCamera.xyz);
		visibility = exp(-pow((distance*density), gradient));
		visibility = clamp(visibility, 0.0, 1.0);
		//
	}

}