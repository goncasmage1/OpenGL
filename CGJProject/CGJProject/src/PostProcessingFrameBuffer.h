#pragma once

#include "FrameBuffer.h"

class PostProcessingFrameBuffer : public FrameBuffer {

protected:

	unsigned int filterBuffer;
	unsigned int filterTexture;
	unsigned int filterDepthBuffer;

	int width, height;

public:

	PostProcessingFrameBuffer();

	void initializePostProcessing(int newWidth, int newHeight);

	void bindFilterFrameBuffer();
	void unbindFrameBuffer();
	void cleanUp();

	unsigned int GetFilterFBO() const { return filterBuffer; }
	unsigned int GetFilterTexture() const { return filterTexture; }

};