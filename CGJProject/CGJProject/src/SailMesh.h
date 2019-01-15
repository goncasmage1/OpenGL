#pragma once

#include <iostream>
#include <vector>
#include "QuadMesh.h"
#include "MeshLoader.h"
#include "NvCloth/Factory.h"
#include "NvCloth/Fabric.h"
#include "NvCloth/Solver.h"
#include "NvCloth/Cloth.h"

class SailMesh : public QuadMesh
{
protected:

	struct SailProperties properties;

	std::vector<Triangle> triangles;
	std::vector<float> inverseMasses;

	void SetupNvCloth();
	void UpdateBuffers();

public:

	SailMesh(SailProperties newProperties, nv::cloth::Factory* newFactory, nv::cloth::Solver* newSolver, float size, int xRepeat, int yRepeat);

	virtual void DestroyBufferObjects() override;
	void UpdateSailData(physx::PxVec3 windVel);
	virtual void Draw() override;

	nv::cloth::Factory* factory = nullptr;
	nv::cloth::Solver* solver = nullptr;
	nv::cloth::Fabric* fabric = nullptr;
	nv::cloth::Cloth* cloth = nullptr;

};