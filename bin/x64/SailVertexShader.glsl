#version 330 core

in vec4 in_Position;
in vec2 in_Coordinates;
in vec3 in_Normal;

out vec4 ex_Color;
out VS_OUT {
	vec3 FragmentPosition;
	vec3 Normal;
	vec2 TextCoords;
}

uniform mat4 ModelMatrix;
uniform vec4 VertColor;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
    vs_out.FragPos = in_Position;
    vs_out.Normal = in_Normal;
    vs_out.TexCoords = in_Coordinates;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
	ex_Color = vec4(0.6, 0.4, 0.0, 1.0);
}