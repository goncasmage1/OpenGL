#pragma once

#include "Mesh.h"

class QuadMesh : public Mesh
{

protected:

	float side;
	int xRepeat, yRepeat;
	void GenerateQuads();

public:

	QuadMesh(float newSide, int newXRepeat, int newYRepeat);

};