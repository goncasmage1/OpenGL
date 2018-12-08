#pragma once
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

struct Triangle
{
	Triangle() {}
	Triangle(uint32_t _a, uint32_t _b, uint32_t _c) :
		a(_a), b(_b), c(_c)
	{
	}
	uint32_t a, b, c;

	static std::vector<Triangle> FromIndices(std::vector<uint32_t> indices)
	{
		std::vector<Triangle> triangles;
		for (int i = 0; i < indices.size(); i += 3)
		{
			triangles.push_back(Triangle(indices[i], indices[i + 1], indices[i + 2]));
		}
		return triangles;
	}

	Triangle operator+(uint32_t offset)const { return Triangle(a + offset, b + offset, c + offset); }
};

class Mesh
{

protected:

	GLuint VAO, VBO, VboVertices;

public:

	int VerticesPerFace = 0;
	bool TexcoordsLoaded, NormalsLoaded;
	std::vector<struct Vec3> Vertices, vertexData, Normals, normalData;
	std::vector<struct Vec2> Texcoords, texcoordData;
	std::vector<uint32_t> vertexIdx, texcoordIdx, normalIdx;

	virtual void CreateBufferObjects();
	virtual void DestroyBufferObjects();
	void Draw();

};