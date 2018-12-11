#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	WIDTH = 0;
	HEIGHT = 0;
	BitDepth = 0;
	fileLocation = '\0';
}

Texture::Texture(std::string fileLoc) {
	textureID = 0;
	WIDTH = 0;
	HEIGHT = 0;
	BitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::LoadTexture() {
	unsigned char *image = stbi_load(fileLocation.c_str(), &WIDTH, &HEIGHT, &BitDepth, 0);

	if (image == NULL) {
		printf("Failed to find: %s\n", fileLocation);
		return;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::UseTexture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture() {
	glDeleteTextures(1, &textureID);
	textureID = 0;
	WIDTH = 0;
	HEIGHT = 0;
	BitDepth = 0;
	fileLocation = '\0';
}

Texture::~Texture()
{
	ClearTexture();
}
