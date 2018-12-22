#pragma once
#include "FrameBuffer.h"

class WaterFrameBuffer : public FrameBuffer {
private:
	/*unsigned int frameBuffer;
	unsigned int Texture;
	unsigned int renderBuffer;*/
	
	
	unsigned int reflectionBuffer;
	unsigned int reflectionTexture;
	unsigned int reflectionDepthBuffer;

	unsigned int refractionBuffer;
	unsigned int refractionTexture;
	unsigned int refractionDepthTexture;
	

	int width, height;

public:
	WaterFrameBuffer() {}
	WaterFrameBuffer(int width, int height);
	void initializeWater(int width, int height);

	void initializeReflectionBuffer();
	void initializeRefractionBuffer();

	unsigned int getReflectionTexture() { return reflectionTexture; }
	unsigned int getRefractionTexture() { return refractionTexture; }
	unsigned int getRefractionDepthTexture() { return refractionDepthTexture; }

	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	void unbindFrameBuffer();
	void cleanUp();
};