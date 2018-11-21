#pragma once

#include "Mesh.h"

class QuadMesh : public Mesh
{

protected:

	void GenerateQuads(float side, int division);

public:

	QuadMesh(float side, int division);

};