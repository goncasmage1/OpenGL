#pragma once

#include <iostream>
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

	void SetupNvCloth();

public:

	SailMesh(nv::cloth::Factory* newFactory, nv::cloth::Solver* newSolver, float size, int xRepeat, int yRepeat);

	virtual void DestroyBufferObjects() override;

	nv::cloth::Factory* factory = nullptr;
	nv::cloth::Solver* solver = nullptr;
	nv::cloth::Fabric* fabric = nullptr;
	nv::cloth::Cloth* cloth = nullptr;

};