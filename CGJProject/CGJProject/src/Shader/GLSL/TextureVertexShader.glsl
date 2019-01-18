#version 330 core

in vec4 in_Position;
in vec2 in_Coordinates;
in vec3 in_Normal;
in vec3 tangent;
in vec3 bitangent;

uniform mat4 ModelMatrix;
uniform vec4 plane;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

out float visibility;

const float density = 0.03;
const float gradient = 1.5;

void main(void)
{


	vec4 worldPosition = ModelMatrix * in_Position;
	vec4 positionRelativeCamera = ViewMatrix * worldPosition;
	gl_Position = ProjectionMatrix * positionRelativeCamera;
    vs_out.FragPos = vec3(worldPosition);   
    vs_out.TexCoords = in_Coordinates;

	//Fog
	float distance = length(positionRelativeCamera.xyz);
	visibility = exp(-pow((distance*density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);
	//
    
    mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * in_Normal);    
    
    mat3 TBN = transpose(mat3(T, B, N));  
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

	gl_ClipDistance[0] = dot(worldPosition, plane);	
}