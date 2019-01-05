#version 330 core

in vec4 in_Position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 FragPos;
out vec3 cameraPos;
out vec3 lightPos;
out vec3 cameraDir;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 cameraDirection;

uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

const float tilling = 0.25f;

void main(void)
{
	vec4 worldPosition = ModelMatrix * in_Position;

	clipSpace = ProjectionMatrix *  ViewMatrix * worldPosition;

	gl_Position = clipSpace;
	textureCoords = vec2(in_Position.x/2.0 + 0.5, in_Position.z/2.0 + 0.5)*tilling;
	
	FragPos = worldPosition.xyz;	
	cameraPos = cameraPosition;
	//cameraPos = cameraPosition;
	
	lightPos = normalize(worldPosition.xyz - lightPosition);
	//lightPos = normalize((ModelMatrix * aux).xyz);

	cameraDir = cameraDirection;
}