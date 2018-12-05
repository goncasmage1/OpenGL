#include "MeshLoader.h"
#include "Mesh.h"
#include "QuadMesh.h"
#include "SailMesh.h"
#include "Math/Vector.h"
#include <sstream>
#include <fstream>
#include <algorithm>

void MeshLoader::ParseVertex(std::stringstream& sin)
{
	Vec3 v;
	sin >> v;
	TempMeshRef->vertexData.push_back(v);
}

void MeshLoader::ParseTexcoord(std::stringstream& sin)
{
	Vec2 t;
	sin >> t;
	TempMeshRef->texcoordData.push_back(t);
}

void MeshLoader::ParseNormal(std::stringstream& sin)
{
	Vec3 n;
	sin >> n;
	TempMeshRef->normalData.push_back(n);
}

void MeshLoader::ParseFace(std::stringstream& sin)
{
	if (TempMeshRef->VerticesPerFace == 0)
	{
		int len = sin.tellg();
		std::string line;
		std::getline(sin, line);
		size_t barCount = std::count(line.begin(), line.end(), '/');
		TempMeshRef->VerticesPerFace = barCount/2;
		sin.seekg(len, std::ios_base::beg);
	}

	std::string token;
	for (int i = 0; i < TempMeshRef->VerticesPerFace; i++)
	{
		std::getline(sin, token, '/');
		if (token.size() > 0) TempMeshRef->vertexIdx.push_back(std::stoi(token));
		std::getline(sin, token, '/');
		if (token.size() > 0) TempMeshRef->texcoordIdx.push_back(std::stoi(token));
		std::getline(sin, token, ' ');
		if (token.size() > 0) TempMeshRef->normalIdx.push_back(std::stoi(token));
	}
}

void MeshLoader::ParseLine(std::stringstream& sin)
{
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) ParseVertex(sin);
	else if (s.compare("vt") == 0) ParseTexcoord(sin);
	else if (s.compare("vn") == 0) ParseNormal(sin);
	else if (s.compare("f") == 0) ParseFace(sin);
}

void MeshLoader::LoadMeshData(const std::string& filename)
{
	std::ifstream ifile(filename);
	std::string line;
	while (std::getline(ifile, line))
	{
		std::stringstream ss = std::stringstream(line);
		ParseLine(ss);
	}
	TempMeshRef->TexcoordsLoaded = (TempMeshRef->texcoordIdx.size() > 0);
	TempMeshRef->NormalsLoaded = (TempMeshRef->normalIdx.size() > 0);
}

void MeshLoader::ProcessMeshData()
{
	for (unsigned int i = 0; i < TempMeshRef->vertexIdx.size(); i++)
	{
		unsigned int vi = TempMeshRef->vertexIdx[i];
		Vec3 v = TempMeshRef->vertexData[vi - 1];
		TempMeshRef->Vertices.push_back(v);
		if (TempMeshRef->TexcoordsLoaded)
		{
			unsigned int ti = TempMeshRef->texcoordIdx[i];
			Vec2 t = TempMeshRef->texcoordData[ti - 1];
			TempMeshRef->Texcoords.push_back(t);
		}
		if (TempMeshRef->NormalsLoaded)
		{
			unsigned int ni = TempMeshRef->normalIdx[i];
			Vec3 n = TempMeshRef->normalData[ni - 1];
			TempMeshRef->Normals.push_back(n);
		}
	}
}

void MeshLoader::FreeMeshData()
{
	TempMeshRef->vertexData.clear();
	TempMeshRef->texcoordData.clear();
	TempMeshRef->normalData.clear();
	TempMeshRef->vertexIdx.clear();
	TempMeshRef->texcoordIdx.clear();
	TempMeshRef->normalIdx.clear();
	TempMeshRef = nullptr;
}

std::shared_ptr<Mesh> MeshLoader::CreateMesh(const std::string& filename)
{
	std::shared_ptr<Mesh> LoadedMesh = std::make_shared<Mesh>();
	TempMeshRef = LoadedMesh;
	Meshes.push_back(LoadedMesh);
	LoadMeshData(filename);
	ProcessMeshData();
	FreeMeshData();
	return LoadedMesh;
}

std::shared_ptr<QuadMesh> MeshLoader::CreateQuadMesh(float size, int xRepeat, int yRepeat)
{
	std::shared_ptr<QuadMesh> newQuad = std::make_shared<QuadMesh>(size, xRepeat, yRepeat);
	Meshes.push_back(newQuad);
	return newQuad;
}

std::shared_ptr<class SailMesh> MeshLoader::CreateSailMesh(nv::cloth::Factory * newFactory, nv::cloth::Solver* newSolver, float size, int xRepeat, int yRepeat)
{
	std::shared_ptr<SailMesh> newSail = std::make_shared<SailMesh>(newFactory, newSolver, size, xRepeat, yRepeat);
	Meshes.push_back(newSail);
	Sails.push_back(newSail);
	return newSail;
}

void MeshLoader::CreateBufferObjects()
{
	for (std::shared_ptr<Mesh> mesh : Meshes)
	{
		mesh->CreateBufferObjects();
	}
}

void MeshLoader::DestroyBufferObjects()
{
	for (std::shared_ptr<Mesh> mesh : Meshes)
	{
		mesh->DestroyBufferObjects();
	}
}

void MeshLoader::UpdateSailData(physx::PxVec3 windVel)
{
	for (std::shared_ptr<SailMesh> sail : Sails)
	{
		sail->cloth->setWindVelocity(windVel);
		nv::cloth::MappedRange<physx::PxVec4> particles = sail->cloth->getCurrentParticles();
		for (size_t i = 0; i < particles.size(); i++)
		{
			//do something with particles[i]
			//the xyz components are the current positions
			//the w component is the invMass.
		}
	}
}
