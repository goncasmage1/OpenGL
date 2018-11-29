#pragma once

#include "Mesh.h"

class QuadMesh : public Mesh
{

protected:

	void GenerateQuads(float side, int xRepeat, int yRepeat);

public:

	QuadMesh(float side, int xRepeat, int yRepeat);

};