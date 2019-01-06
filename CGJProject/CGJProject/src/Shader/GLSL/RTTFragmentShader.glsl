#version 330 core

in vec2 ex_textCoord;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{
	color = texture(screenTexture, ex_textCoord);
}