#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>


class FrameBuffer {
private:
	unsigned int width, height;
public:


	FrameBuffer() {  }
	FrameBuffer(int width, int height);
	int createFrameBuffer();
	int createTextureAttachment(int width, int height, GLboolean depth, GLboolean stencil);
	int createDepthTextureAttachment(int width, int height); //
	int createDepthBufferAttachment(int width, int height); //
	GLuint createRenderBuffer(int width, int height);
	GLuint loadTexture(const GLchar* path, GLboolean alpha);
	void bindFrameBuffer(unsigned int frameBuffer, int width, int height);
	void unbindCurrentFrameBuffer(int width, int height);
	void virtual cleanUp() = 0;

};