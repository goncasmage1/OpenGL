#version 330 core

in vec4 in_Position;
in vec2 in_Coordinates;
in vec3 in_Normal;
in vec3 tangent;
in vec3 bitangent;
//out vec4 ex_Color;
out vec2 ex_textCoord;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	//vec2 TexCoords;
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

void main(void)
{
	vec4 worldPosition = ModelMatrix * in_Position;
	gl_Position = ProjectionMatrix * ViewMatrix * worldPosition;
	vs_out.FragPos = vec3(ModelMatrix * in_Position);
	ex_textCoord = in_Coordinates;

	mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
	vs_out.Normal = normalMatrix * in_Normal;

	vec3 T = normalize(vec3(ModelMatrix * vec4(tangent, 0.0)));
	vec3 N = normalize(vec3(ModelMatrix * vec4(in_Normal, 0.0)));

	T = normalize(T - dot(T, N) * N);

	vec3 B = cross(T, N);

	mat3 TBN = mat3(T, B, N);

	vs_out.TangentLightPos = TBN * lightPos;
	vs_out.TangentViewPos = TBN * viewPos;
	vs_out.TangentFragPos = TBN * vec3(ModelMatrix * in_Position);
	//gl_ClipDistance[0] = dot(worldPosition, plane);
	gl_ClipDistance[0] = dot(worldPosition, plane);
	
}