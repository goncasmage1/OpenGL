#pragma once
#include <vector>

class Mesh
{

protected:


public:

	bool TexcoordsLoaded, NormalsLoaded;
	std::vector <struct Vec3> Vertices, vertexData, Normals, normalData;
	std::vector <struct Vec2> Texcoords, texcoordData;
	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

	Mesh();
};