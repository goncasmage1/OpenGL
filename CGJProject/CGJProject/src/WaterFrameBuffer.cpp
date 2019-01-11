#include "WaterFrameBuffer.h"

WaterFrameBuffer::WaterFrameBuffer(int width, int height)
{

}

void WaterFrameBuffer::initializeWater(int width, int height)
{
	this->width = width;
	this->height = height;
	initializeReflectionBuffer();
	initializeRefractionBuffer();
}

void WaterFrameBuffer::initializeReflectionBuffer()
{
	reflectionBuffer = createFrameBuffer();
	reflectionTexture = createTextureAttachment(width, height, false, false);
	reflectionDepthBuffer = createDepthBufferAttachment(width, height);
	unbindCurrentFrameBuffer(width, height);
}

void WaterFrameBuffer::initializeRefractionBuffer()
{
	refractionBuffer = createFrameBuffer();
	refractionTexture = createTextureAttachment(width, height, false, false);
	refractionDepthTexture = createDepthTextureAttachment(width, height);
	unbindCurrentFrameBuffer(width, height);
}


void WaterFrameBuffer::bindReflectionFrameBuffer()
{
	FrameBuffer::bindFrameBuffer(reflectionBuffer, width, height);
}

void WaterFrameBuffer::bindRefractionFrameBuffer()
{
	FrameBuffer::bindFrameBuffer(refractionBuffer, width, height);
}

void WaterFrameBuffer::unbindFrameBuffer()
{
	FrameBuffer::unbindCurrentFrameBuffer(width, height);
}


void WaterFrameBuffer::cleanUp()
{
	glDeleteFramebuffers(1, &reflectionBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);

	glDeleteFramebuffers(1, &refractionBuffer);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionDepthTexture);
}
