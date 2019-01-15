#version 330 core

in vec4 in_Position;
in vec2 in_Coordinates;
in vec3 in_Normal;
in vec3 tangent;
in vec3 bitangent;

out vec2 ex_textCoord;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec4 LightVec;
	vec4 ViewVec;
} vs_out;

uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform vec4 plane;

uniform vec3 lightPos;
uniform vec3 viewPos;
out vec3 worldLight;
out mat3 TBN;
void main(void)
{

	vec4 worldPosition = ModelMatrix * in_Position;
	gl_Position = ProjectionMatrix * ViewMatrix * worldPosition;
	vs_out.FragPos = vec3(ViewMatrix * ModelMatrix * in_Position);
	ex_textCoord = in_Coordinates;

	mat3 normalMatrix = transpose(inverse(mat3(ViewMatrix * ModelMatrix)));
	vs_out.Normal = normalMatrix * in_Normal;

	vec3 T = normalize((normalMatrix * tangent).xyz);
	vec3 B = normalize((normalMatrix * bitangent).xyz);
	vec3 N = normalize((normalMatrix * in_Normal).xyz);

	TBN = transpose(mat3(T,B,N));

	vs_out.LightVec = vec4()

	worldLight = vec3(ViewMatrix * vec4(lightPos, 1.0));
    //vs_out.LightVec = vec4(normalize(lightPos - worldPosition.xyz), 0.0);
    //vs_out.ViewVec  = vec4(normalize(viewPos - worldPosition.xyz), 0.0);
	//vs_out.TangentFragPos = TBN * vec3(worldPosition);
	gl_ClipDistance[0] = dot(worldPosition, plane);
	
}