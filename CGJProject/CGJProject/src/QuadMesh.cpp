#include "QuadMesh.h"
#include "Math/Vector.h"
#include <assert.h>

QuadMesh::QuadMesh(float side, int xRepeat, int yRepeat)
{
	VerticesPerFace = 3;
	TexcoordsLoaded = true;
	GenerateQuads(side, xRepeat, yRepeat);
}

void QuadMesh::GenerateQuads(float side, int xRepeat, int yRepeat)
{
	assert(side > 0.f);
	assert(xRepeat > 0);
	assert(yRepeat > 0);

	float halfXSize = (side * xRepeat) / 2.f;
	float halfYSize = (side * yRepeat) / 2.f;

	Vertices.empty();
	Normals.empty();

	for (int i = 0; i < yRepeat; i++)
	{
		for (int j = 0; j < xRepeat; j++)
		{
			Vertices.push_back(Vec3(-halfXSize + j * side, -halfYSize + i * side, 0.f));
			Texcoords.push_back(Vec2((float)j / (float)xRepeat, (float)i / (float)yRepeat));
			Vertices.push_back(Vec3(-halfXSize + (j + 1) * side, -halfYSize + i * side, 0.f));
			Texcoords.push_back(Vec2((float)(j + 1) / (float)xRepeat, (float)i / (float)yRepeat));
			Vertices.push_back(Vec3(-halfXSize + j * side, -halfYSize + (i + 1) * side, 0.f));
			Texcoords.push_back(Vec2((float)j / (float)xRepeat, (float)(i + 1) / (float)yRepeat));
			Vertices.push_back(Vec3(-halfXSize + j * side, -halfYSize + (i + 1) * side, 0.f));
			Texcoords.push_back(Vec2((float)j / (float)xRepeat, (float)(i + 1) / (float)yRepeat));
			Vertices.push_back(Vec3(-halfXSize + (j + 1) * side, -halfYSize + i * side, 0.f));
			Texcoords.push_back(Vec2((float)(j + 1) / (float)xRepeat, (float)i / (float)yRepeat));
			Vertices.push_back(Vec3(-halfXSize + (j + 1) * side, -halfYSize + (i + 1) * side, 0.f));
			Texcoords.push_back(Vec2((float)(j + 1) / (float)xRepeat, (float)(i + 1) / (float)yRepeat));

			for (int k = 0; k < 6; k++) Normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
		}
	}
}
