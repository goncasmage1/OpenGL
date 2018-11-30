/*
* Copyright (c) 2008-2017, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#include "PlaneCollisionScene.h"
#include "scene/SceneController.h"
#include <NvClothExt/ClothFabricCooker.h>
#include "ClothMeshGenerator.h"
#include <NvCloth/Fabric.h>
#include <NvCloth/Solver.h>
#include <NvCloth/Cloth.h>
#include <NvCloth/Factory.h>
#include "Renderer.h"
#include "renderer/RenderUtils.h"
#include "windows.h"
#include "utils/MeshGenerator.h"

DECLARE_SCENE_NAME(PlaneCollisionScene, "Plane Collision Scene")


void PlaneCollisionScene::initializeCloth(int index, physx::PxVec3 offset)
{
	///////////////////////////////////////////////////////////////////////
	ClothMeshData clothMesh;

	physx::PxMat44 transform = PxTransform(PxVec3(0.f, 13.f, 0.f)+ offset, PxQuat(0, PxVec3(1.f, 0.f, 0.f)));
	clothMesh.GeneratePlaneCloth(5.f, 6.f, 59, 69, false, transform);
	clothMesh.AttachClothPlaneByAngles(59, 69);
	clothMesh.SetInvMasses(0.5f + (float)index * 2.0f);

	mClothActor[index] = new ClothActor;
	nv::cloth::ClothMeshDesc meshDesc = clothMesh.GetClothMeshDesc();
	{
		mClothActor[index]->mClothRenderMesh = new ClothRenderMesh(meshDesc);
		mClothActor[index]->mClothRenderable = getSceneController()->getRenderer().createRenderable(*(static_cast<IRenderMesh*>(mClothActor[index]->mClothRenderMesh)), *getSceneController()->getDefaultMaterial());

		float r, g, b;
		r = index == 0 ? 1.0f : 0.3f;
		g = index == 1 ? 1.0f : 0.3f;
		b = index == 2 ? 1.0f : 0.3f;

		mClothActor[index]->mClothRenderable->setColor(DirectX::XMFLOAT4(r, g, b, 1.0f));
	}

	nv::cloth::Vector<int32_t>::Type phaseTypeInfo;
	mFabric[index] = NvClothCookFabricFromMesh(getSceneController()->getFactory(), meshDesc, physx::PxVec3(0.0f, 0.0f, 1.0f), &phaseTypeInfo, false);
	trackFabric(mFabric[index]);

	// Initialize start positions and masses for the actual cloth instance
	// (note: the particle/vertex positions do not have to match the mesh description here. Set the positions to the initial shape of this cloth instance)
	std::vector<physx::PxVec4> particlesCopy;
	particlesCopy.resize(clothMesh.mVertices.size());

	physx::PxVec3 clothOffset = transform.getPosition();
	for(int i = 0; i < (int)clothMesh.mVertices.size(); i++)
	{
		// To put attachment point closer to each other
		if(clothMesh.mInvMasses[i] < 1e-6)
			clothMesh.mVertices[i] = (clothMesh.mVertices[i] - clothOffset)*0.95f + clothOffset;

		particlesCopy[i] = physx::PxVec4(clothMesh.mVertices[i], clothMesh.mInvMasses[i]); // w component is 1/mass, or 0.0f for anchored/fixed particles
	}

	// Create the cloth from the initial positions/masses and the fabric
	mClothActor[index]->mCloth = getSceneController()->getFactory()->createCloth(nv::cloth::Range<physx::PxVec4>(&particlesCopy[0], &particlesCopy[0] + particlesCopy.size()), *mFabric[index]);
	particlesCopy.clear(); particlesCopy.shrink_to_fit();

	mClothActor[index]->mCloth->setGravity(physx::PxVec3(0.0f, -9.8f, 0.0f));
	mClothActor[index]->mCloth->setDamping(physx::PxVec3(0.1f, 0.1f, 0.1f));

	//Generate collision planes
	std::vector<physx::PxVec4> planes;
	planes.push_back(physx::PxVec4(physx::PxVec3(-0.5f, 0.4f, 0.0f).getNormalized(), -4.0f));
	planes.push_back(physx::PxVec4(physx::PxVec3(0.0f, 0.4f, 0.5f).getNormalized(), -4.0f));
	uint32_t mask = 3;
	MeshGenerator::Mesh mesh = MeshGenerator::generateCollisionConvex(planes.data(), mask, -0.01f, true);

	//assign as collision data
	nv::cloth::Range<const physx::PxVec4> planesR(&planes[0], &planes[0] + planes.size());
	mClothActor[index]->mCloth->setPlanes(planesR, 0, mClothActor[index]->mCloth->getNumPlanes());

	//assign convex indices
	std::vector<uint32_t> indices;
	indices.push_back(1);
	indices.push_back(2);
	nv::cloth::Range<uint32_t> cind(&indices[0], &indices[0] + indices.size());
	mClothActor[index]->mCloth->setConvexes(cind, 0, mClothActor[index]->mCloth->getNumConvexes());

	//create render mesh
	//scale down the render model to not intersect the triangles so much
	auto renderMesh = new MeshGenerator::MeshGeneratorRenderMesh(mesh);
	Renderable* renderable = getSceneController()->getRenderer().createRenderable(*renderMesh, *getSceneController()->getDefaultMaterial());
	trackRenderable(renderable);

	// Setup phase configs
	std::vector<nv::cloth::PhaseConfig> phases(mFabric[index]->getNumPhases());
	for(int i = 0; i < (int)phases.size(); i++)
	{
		phases[i].mPhaseIndex = i;
		phases[i].mStiffness = 1.0f;
		phases[i].mStiffnessMultiplier = 1.0f;
		phases[i].mCompressionLimit = 1.0f;
		phases[i].mStretchLimit = 1.0f;
	}
	mClothActor[index]->mCloth->setPhaseConfig(nv::cloth::Range<nv::cloth::PhaseConfig>(&phases.front(), &phases.back()));
	mClothActor[index]->mCloth->setDragCoefficient(0.1f);
	mClothActor[index]->mCloth->setLiftCoefficient(0.1f);

	trackClothActor(mClothActor[index]);

	// Add the cloth to the solver for simulation
	addClothToSolver(mClothActor[index], mSolver);
}

void PlaneCollisionScene::onInitialize()
{
	mSolver = getSceneController()->getFactory()->createSolver();
	trackSolver(mSolver);

	initializeCloth(0, physx::PxVec3(0.0f, 0.0f, 0.0f));

	{
		IRenderMesh* mesh = getSceneController()->getRenderer().getPrimitiveRenderMesh(PrimitiveRenderMeshType::Plane);
		Renderable* plane = getSceneController()->getRenderer().createRenderable(*mesh, *getSceneController()->getDefaultPlaneMaterial());
		plane->setTransform(PxTransform(PxVec3(0.f, 0.f, 0.f), PxQuat(PxPiDivTwo, PxVec3(0.f, 0.f, 1.f))));
		plane->setScale(PxVec3(1000.f));
		trackRenderable(plane);
	}
}
