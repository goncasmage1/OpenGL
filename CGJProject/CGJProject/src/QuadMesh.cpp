#include "QuadMesh.h"
#include "Math/Vector.h"
#include <assert.h>

QuadMesh::QuadMesh(float newSide, int newXRepeat, int newYRepeat) :
	Mesh(), side(newSide), xRepeat(newXRepeat), yRepeat(newYRepeat)
{
	VerticesPerFace = 3;
	TexcoordsLoaded = true;
	GenerateQuads();
}

void QuadMesh::GenerateQuads()
{
	assert(side > 0.f);
	assert(xRepeat > 0);
	assert(yRepeat > 0);

	float halfXSize = (side * xRepeat) / 2.f;
	float halfYSize = (side * yRepeat) / 2.f;

	Normals.empty();
	vertexIdx.empty();
	vertexData.empty();

	for (int i = 0; i < yRepeat + 1; i++)
	{
		for (int j = 0; j < xRepeat; j++)
		{
			Vec3 BottomLeft = Vec3(-halfXSize + j * side, 0.f, -halfYSize + i * side);
			Vec3 BottomRight = Vec3(-halfXSize + (j + 1) * side, 0.f, -halfYSize + i * side);

			if (j == 0)
			{
				vertexData.push_back(BottomLeft);
				vertexData.push_back(BottomRight);
			}
			else
			{
				vertexData.push_back(BottomRight);
			}

			if (i != yRepeat)
			{
				Vec3 TopLeft = Vec3(-halfXSize + j * side, 0.f, -halfYSize + (i + 1) * side);
				Vec3 TopRight = Vec3(-halfXSize + (j + 1) * side, 0.f, -halfYSize + (i + 1) * side);

				vertexIdx.push_back(j + i * (xRepeat + 1));
				Texcoords.push_back(Vec2((float)j / (float)xRepeat, (float)i / (float)yRepeat));

				vertexIdx.push_back((j + 1) + i * (xRepeat + 1));
				Texcoords.push_back(Vec2((float)(j + 1) / (float)xRepeat, (float)i / (float)yRepeat));

				vertexIdx.push_back(j + (i + 1) * (xRepeat + 1));
				Texcoords.push_back(Vec2((float)j / (float)xRepeat, (float)(i + 1) / (float)yRepeat));

				vertexIdx.push_back(j + (i + 1) * (xRepeat + 1));
				Texcoords.push_back(Vec2((float)j / (float)xRepeat, (float)(i + 1) / (float)yRepeat));

				vertexIdx.push_back((j + 1) + i * (xRepeat + 1));
				Texcoords.push_back(Vec2((float)(j + 1) / (float)xRepeat, (float)i / (float)yRepeat));

				vertexIdx.push_back((j + 1) + (i + 1) * (xRepeat + 1));
				Texcoords.push_back(Vec2((float)(j + 1) / (float)xRepeat, (float)(i + 1) / (float)yRepeat));

				for (int k = 0; k < 6; k++) Normals.push_back(Vec3(0.0f, 1.0f, 0.0f));
			}
		}
	}
}
