#version 330 core

in VS_OUT {
	vec3 FragmentPosition;
	vec3 Normal;
	vec2 TextCoords;
} fs_in;

out vec4 out_Color;

uniform sampler2D screenTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void)
{
    vec3 color = texture(screenTexture, fs_in.TextCoords).rgb;
    out_Color = vec4(color, 1.0);
}