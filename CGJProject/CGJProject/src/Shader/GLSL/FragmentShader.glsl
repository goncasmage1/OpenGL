#version 330 core

in vec4 ex_Color;
in vec2 TexCoord;

out vec4 out_Color;

uniform sampler2D ourTexture;

void main(void)
{
	out_Color = texture(ourTexture, TexCoord);
}