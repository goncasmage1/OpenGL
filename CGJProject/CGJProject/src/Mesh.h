#pragma once
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Math/Vector.h"

class Mesh
{

protected:

	GLuint VAO, VBO;

public:

	int VerticesPerFace = 0;
	bool TexcoordsLoaded, NormalsLoaded;
	std::vector<Vec3> Vertices, vertexData, Normals, normalData, tangentData, biTangentData;
	std::vector<Vec2> Texcoords, texcoordData;
	std::vector<unsigned int> vertexIdx, texcoordIdx, normalIdx;

	virtual void CreateBufferObjects();
	virtual void DestroyBufferObjects();
	virtual void Draw();

};