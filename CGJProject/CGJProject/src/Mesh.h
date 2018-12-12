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
	std::vector<GLuint> vertexIdx, texcoordIdx, normalIdx;

	virtual void CreateBufferObjects();
	virtual void DestroyBufferObjects();
	void Draw();

};