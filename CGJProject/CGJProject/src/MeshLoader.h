#pragma once
#include <iostream>
#include <string>
#include <vector>

class MeshLoader
{
protected:

	void ParseVertex(std::stringstream& sin);
	void ParseTexcoord(std::stringstream& sin);
	void ParseNormal(std::stringstream& sin);
	void ParseFace(std::stringstream& sin);
	void ParseLine(std::stringstream& sin);
	void LoadMeshData(const std::string& filename);
	void ProcessMeshData();
	void FreeMeshData();
	
	std::shared_ptr<class Mesh> TempMeshRef = nullptr;


public:

	std::vector<std::shared_ptr<class Mesh>> Meshes;

	std::shared_ptr<class Mesh> CreateMesh(const std::string& filename);
	std::shared_ptr<class QuadMesh> CreateQuadMesh(float size, int division);
	void CreateBufferObjects();
	void DestroyBufferObjects();

};