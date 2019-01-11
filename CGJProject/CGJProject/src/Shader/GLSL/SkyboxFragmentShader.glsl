#version 330 core

out vec4 color;

in vec3 textureDir; // Direction vector representing a 3D texture coordinate
uniform samplerCube skybox; // Cubemap texture sampler

void main()
{
	color = texture(skybox, textureDir);
}