#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "NvCloth/Factory.h"
#include "NvCloth/Cloth.h"
#include "foundation/PxVec3.h"

struct SailProperties
{
	SailProperties();
	SailProperties(physx::PxVec3 newGravity, float newDragCoefficient, float newLiftCoefficient, nv::cloth::PhaseConfig newPhaseConfig);

	physx::PxVec3 gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
	float dragCoefficient = 0.5f;
	float liftCoefficient = 0.6f;

	nv::cloth::PhaseConfig phaseConfig = nv::cloth::PhaseConfig();
};

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
	std::vector<std::shared_ptr<class SailMesh>> Sails;

	std::shared_ptr<class Mesh> CreateMesh(const std::string& filename);
	std::shared_ptr<class QuadMesh> CreateQuadMesh(float size, int xRepeat, int yRepeat);
	std::shared_ptr<class SailMesh> CreateSailMesh(SailProperties properties, nv::cloth::Factory* newFactory, nv::cloth::Solver* newSolver, float size, int xRepeat, int yRepeat);

	void CreateBufferObjects();
	void DestroyBufferObjects();

	void UpdateSailData(physx::PxVec3 windVel);
};