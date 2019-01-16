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

void main(void)
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
    vs_out.FragPos = vec3(ModelMatrix * in_Position);   
    vs_out.TexCoords = in_Coordinates;
    
    mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * in_Normal);    
    
    mat3 TBN = transpose(mat3(T, B, N));  
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

	gl_ClipDistance[0] = dot(ModelMatrix * in_Position, plane);	
}