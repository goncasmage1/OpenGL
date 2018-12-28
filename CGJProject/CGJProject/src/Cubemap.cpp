#include "Cubemap.h"
#include <cassert>

void Cubemap::initializeFrameBuffer(int width, int height, std::vector<const char*> faces)
{
	this->width = width;
	this->height = height;

	//LoadCubeMap(faces);

}

