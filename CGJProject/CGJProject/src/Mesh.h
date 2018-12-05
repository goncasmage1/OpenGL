#pragma once
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

struct Triangle
{
	Triangle() {}
	Triangle(uint16_t _a, uint16_t _b, uint16_t _c) :
		a(_a), b(_b), c(_c)
	{
	}
	uint16_t a, b, c;

	static std::vector<Triangle> FromIndices(std::vector<uint16_t> indices)
	{
		std::vector<Triangle> triangles;
		for (int i = 0; i < indices.size(); i += 3)
		{
			triangles.push_back(Triangle(i, i + 1, i + 2));
		}
		return triangles;
	}

	Triangle operator+(uint32_t offset)const { return Triangle(a + offset, b + offset, c + offset); }
};

class Mesh
{

protected:

	GLuint VAO, VBO;

public:

	int VerticesPerFace = 0;
	bool TexcoordsLoaded, NormalsLoaded;
	std::vector<struct Vec3> Vertices, vertexData, Normals, normalData;
	std::vector<struct Vec2> Texcoords, texcoordData;
	std::vector<uint16_t> vertexIdx, texcoordIdx, normalIdx;

	virtual void CreateBufferObjects();
	virtual void DestroyBufferObjects();
	void Draw();

};