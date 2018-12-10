#include "SailMesh.h"

#include "NvClothExt/ClothFabricCooker.h"

#include "PxCallbacks/CustomAllocator.h"
#include "PxCallbacks/CustomAssertHandler.h"
#include "PxCallbacks/CustomError.h"
#include "PxCallbacks/CustomProfiler.h"

#include "Math/Vector.h"

SailMesh::SailMesh(nv::cloth::Factory* newFactory, nv::cloth::Solver* newSolver, float size, int xRepeat, int yRepeat) : QuadMesh(size, xRepeat, yRepeat)
{
	factory = newFactory;
	solver = newSolver;
	gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
	dragCoefficient = 0.5f;
	liftCoefficient = 0.6f;

	triangles = Triangle::FromIndices(vertexIdx);

	SetupNvCloth();
}

void SailMesh::DestroyBufferObjects()
{
	QuadMesh::DestroyBufferObjects();

	solver->removeCloth(cloth);
	NV_CLOTH_DELETE(cloth);

	fabric->decRefCount();
}

void SailMesh::SetupNvCloth()
{
	nv::cloth::ClothMeshDesc meshDesc;
	size_t i = 0;

	meshDesc.setToDefault();
	meshDesc.points.data = &vertexData[0];
	meshDesc.points.stride = sizeof(vertexData[0]);
	meshDesc.points.count = (physx::PxU32)vertexData.size();

	meshDesc.triangles.data = &triangles[0];
	meshDesc.triangles.stride = sizeof(triangles[0]);
	meshDesc.triangles.count = (physx::PxU32)triangles.size();

	for (i = 0; i < vertexData.size(); i++) inverseMasses.push_back(1.f);
	//inverseMasses[0] = 0.f;
	//inverseMasses[yRepeat-1] = 0.f;

	meshDesc.invMasses.data = &inverseMasses[0];
	meshDesc.invMasses.stride = sizeof(inverseMasses[0]);
	meshDesc.invMasses.count = (physx::PxU32)inverseMasses.size();

	physx::PxVec3 gravity(0.0f, -9.8f, 0.0f);
	nv::cloth::Vector<int32_t>::Type phaseTypeInfo;
	fabric = NvClothCookFabricFromMesh(factory, meshDesc, gravity, &phaseTypeInfo);

	std::vector<physx::PxVec4> particles;
	particles.resize(meshDesc.points.count);
	for (i = 0; i < meshDesc.points.count; i++)
	{
		Vec4 vec = vertexData[i];
		float invMass = i == 0 || i == (xRepeat+1) * (yRepeat) ? 0.f : 1.0f;
		particles[i] = physx::PxVec4(vec.x, vec.y, vec.z, invMass);
	}

	cloth = factory->createCloth(
	nv::cloth::Range<physx::PxVec4>(&particles.front(),
									&particles.back() + 1),
									*fabric);

	cloth->setGravity(gravity);
	cloth->setDragCoefficient(dragCoefficient);
	cloth->setLiftCoefficient(liftCoefficient);

	nv::cloth::PhaseConfig* phases = new nv::cloth::PhaseConfig[fabric->getNumPhases()];
	for (i = 0; i < fabric->getNumPhases(); i++)
	{
		phases[i].mPhaseIndex = (uint16_t)i; // Set index to the corresponding set (constraint group)

		//Give phases different configs depending on type
		switch (phaseTypeInfo[i])
		{
			case nv::cloth::ClothFabricPhaseType::eINVALID:
				//ERROR
				break;
			case nv::cloth::ClothFabricPhaseType::eVERTICAL:
				break;
			case nv::cloth::ClothFabricPhaseType::eHORIZONTAL:
				break;
			case nv::cloth::ClothFabricPhaseType::eBENDING:
				break;
			case nv::cloth::ClothFabricPhaseType::eSHEARING:
				break;
		}

		//For this example we give every phase the same config
		phases[i].mStiffness = 1.0f;
		phases[i].mStiffnessMultiplier = 1.0f;
		phases[i].mCompressionLimit = 1.0f;
		phases[i].mStretchLimit = 1.0f;
	}
	cloth->setPhaseConfig(nv::cloth::Range<nv::cloth::PhaseConfig>(phases, phases + fabric->getNumPhases()));
	delete[] phases;

	//Add the cloth to the solver for simulation
	solver->addCloth(cloth);
}

void SailMesh::UpdateSailData(physx::PxVec3 windVel)
{
	cloth->setWindVelocity(windVel);
	nv::cloth::MappedRange<physx::PxVec4> newParticles = cloth->getCurrentParticles();
	for (size_t i = 0; i < vertexIdx.size(); i++)
	{
		physx::PxVec4 newParticle = newParticles[vertexIdx[i]];
		Vertices[i] = Vec3(newParticle.x, newParticle.y, newParticle.z);
	}
	UpdateBuffers();
}

void SailMesh::UpdateBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vec3), &Vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}