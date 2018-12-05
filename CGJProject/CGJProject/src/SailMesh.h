#pragma once

#include <iostream>
#include <vector>
#include "QuadMesh.h"
#include "NvCloth/Factory.h"
#include "NvCloth/Fabric.h"
#include "NvCloth/Solver.h"
#include "NvCloth/Cloth.h"

class SailMesh : public QuadMesh
{
protected:

	physx::PxVec3 gravity;
	float dragCoefficient;
	float liftCoefficient;

	std::vector<Triangle> triangles;
	std::vector<float> inverseMasses;

	void SetupNvCloth();

public:

	SailMesh(nv::cloth::Factory* newFactory, nv::cloth::Solver* newSolver, float size, int xRepeat, int yRepeat);

	virtual void DestroyBufferObjects() override;

	void UpdateSailData(physx::PxVec3 windVel);

	nv::cloth::Factory* factory = nullptr;
	nv::cloth::Solver* solver = nullptr;
	nv::cloth::Fabric* fabric = nullptr;
	nv::cloth::Cloth* cloth = nullptr;

};