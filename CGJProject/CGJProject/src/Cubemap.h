#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "SOIL.h"

#include <vector>

#include "FrameBuffer.h"



class Cubemap : public FrameBuffer {
private:
	unsigned int frameBuffer;
	unsigned int Texture;
	unsigned int renderBuffer;

	int width, height;
public:
	Cubemap() {}
	void initializeFrameBuffer(int width, int height, std::vector<const char*> faces);
	void bindFrameBuffer();
	void unbindFrameBuffer();
	void cleanUp();
};