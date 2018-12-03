#include "SailMesh.h"

#include "NvClothExt/ClothFabricCooker.h"
//#include "cuda.h"

#include "PxCallbacks/CustomAllocator.h"
#include "PxCallbacks/CustomAssertHandler.h"
#include "PxCallbacks/CustomError.h"
#include "PxCallbacks/CustomProfiler.h"

#include "Math/Vector.h"

SailMesh::SailMesh(nv::cloth::Factory* newFactory, nv::cloth::Solver* newSolver, float size, int xRepeat, int yRepeat) : QuadMesh(size, xRepeat, yRepeat)
{
	factory = newFactory;
	gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
	dragCoefficient = 0.5f;
	liftCoefficient = 0.6f;

	SetupNvCloth();
}

void SailMesh::DestroyBufferObjects()
{
	QuadMesh::DestroyBufferObjects();

	solver->removeCloth(cloth);
	NV_CLOTH_DELETE(cloth);

	//TODO: FIX!
	fabric->decRefCount();
}

void SailMesh::SetupNvCloth()
{
	nv::cloth::ClothMeshDesc meshDesc;

	meshDesc.setToDefault();
	meshDesc.points.data = static_cast<void*>(&Vertices);
	meshDesc.points.stride = sizeof(Vec3);
	meshDesc.points.count = Vertices.size();

	meshDesc.triangles.data = static_cast<void*>(&vertexIdx);
	meshDesc.triangles.stride = sizeof(unsigned int);
	meshDesc.triangles.count = vertexIdx.size();

	int i = 0;
	for (i = 0; i < Vertices.size(); i++) inverseMasses.push_back(1.f);

	meshDesc.invMasses.data = static_cast<void*>(&inverseMasses);
	meshDesc.invMasses.stride = sizeof(float);
	meshDesc.invMasses.count = inverseMasses.size();

	physx::PxVec3 gravity(0.0f, -9.8f, 0.0f);
	nv::cloth::Vector<int32_t>::Type phaseTypeInfo;
	fabric = NvClothCookFabricFromMesh(factory, meshDesc, gravity, &phaseTypeInfo);

	std::vector<physx::PxVec4> particles;
	particles.resize(meshDesc.points.count);
	for (int i = 0; i < meshDesc.points.count; i++)
	{
		Vec4 vec = Vertices[i];
		particles[i] = physx::PxVec4(vec.x, vec.y, vec.z, 1.0f);
	}

	cloth = factory->createCloth(
	nv::cloth::Range<physx::PxVec4>(&particles.front(),
									&particles.back() + 1),
									*fabric);
	particles.clear();

	cloth->setGravity(gravity);
	cloth->setDragCoefficient(dragCoefficient);
	cloth->setLiftCoefficient(liftCoefficient);

	nv::cloth::PhaseConfig* phases = new nv::cloth::PhaseConfig[fabric->getNumPhases()];
	for (int i = 0; i < fabric->getNumPhases(); i++)
	{
		phases[i].mPhaseIndex = i; // Set index to the corresponding set (constraint group)

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
