#include "PostProcessingFrameBuffer.h"

PostProcessingFrameBuffer::PostProcessingFrameBuffer()
{
}

void PostProcessingFrameBuffer::initializePostProcessing(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;

	filterBuffer = createFrameBuffer();
	filterTexture = createTextureAttachment(width, height, false, false, true);
	filterDepthBuffer = createDepthBufferAttachment(width, height);
	unbindCurrentFrameBuffer(width, height);
}

void PostProcessingFrameBuffer::bindFilterFrameBuffer()
{
	FrameBuffer::bindFrameBuffer(filterBuffer, width, height);
}

void PostProcessingFrameBuffer::unbindFrameBuffer()
{
	FrameBuffer::unbindCurrentFrameBuffer(width, height);
}

void PostProcessingFrameBuffer::cleanUp()
{
	glDeleteFramebuffers(1, &filterBuffer);
	glDeleteTextures(1, &filterTexture);
	glDeleteRenderbuffers(1, &filterDepthBuffer);
}
