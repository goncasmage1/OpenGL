#include "QuadMesh.h"
#include "Math/Vector.h"
#include <assert.h>

QuadMesh::QuadMesh(float side, int division)
{
	VerticesPerFace = 3;
	TexcoordsLoaded = false;
	GenerateQuads(side, division);
}

void QuadMesh::GenerateQuads(float side, int division)
{
	assert(side > 0.f);
	assert(division > 0);

	float halfSize = side / 2.f;

	Vertices.empty();
	Normals.empty();

	float quadSize = side / (float)division;

	for (int i = 0; i < division; i++)
	{
		for (int j = 0; j < division; j++)
		{
			Vertices.push_back(Vec3(-halfSize + j * quadSize, -halfSize + i * quadSize, 0.f));
			Vertices.push_back(Vec3(-halfSize + (j + 1) * quadSize, -halfSize + i * quadSize, 0.f));
			Vertices.push_back(Vec3(-halfSize + j * quadSize, -halfSize + (i + 1) * quadSize, 0.f));
			Vertices.push_back(Vec3(-halfSize + j * quadSize, -halfSize + (i + 1) * quadSize, 0.f));
			Vertices.push_back(Vec3(-halfSize + (j + 1) * quadSize, -halfSize + i * quadSize, 0.f));
			Vertices.push_back(Vec3(-halfSize + (j + 1) * quadSize, -halfSize + (i + 1) * quadSize, 0.f));

			for (int k = 0; k < 6; k++) Normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
		}
	}
}
