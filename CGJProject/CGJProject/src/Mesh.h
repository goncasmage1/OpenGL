#pragma once
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

class Mesh
{

protected:

	GLuint VAO, VBO;

public:

	int VerticesPerFace = 0;
	bool TexcoordsLoaded, NormalsLoaded;
	std::vector<struct Vec3> Vertices, vertexData, Normals, normalData;
	std::vector<struct Vec2> Texcoords, texcoordData;
	std::vector<unsigned int> vertexIdx, texcoordIdx, normalIdx;

	void CreateBufferObjects();
	void DestroyBufferObjects();
	void Draw();

};