#include "QuadPolygonMaterial.h"
#include "QuadPolygonMaterialInitializer.h"
#include "QuadPolygonMaterialPreDefine.h"

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>
#include <Bifrost/Resource/Manager/PipelineStateObjectManager.h>

#include <Bifrost/Resource/PSO/PipelineStateObject.h>

USING_BIFROST;
USING_ATLANTIS;
EDENS_NAMESPACE_USING;

CQuadPolygonMaterial::CQuadPolygonMaterial() : Super()
{
}

CQuadPolygonMaterial::~CQuadPolygonMaterial()
{
}

bool CQuadPolygonMaterial::Initialize(const ATLANTIS_NAMESPACE::FMaterialInterfaceInitializerBase* _Initializer)
{
	do
	{
		const FQuadPolygonMaterialInitializer* Initializer = PCast<const FQuadPolygonMaterialInitializer*>(_Initializer);
		CHECK_RESULT_BREAK(Initializer);

		CHECK_RESULT_BREAK(CreateMaterials(Initializer));
		CHECK_RESULT_BREAK(CreatePipelineStateObject(Initializer));
		CHECK_RESULT_BREAK(CreateSceneData(Initializer));

		return true;

	} while (0);

	Finalize();
	return false;
}

void CQuadPolygonMaterial::Finalize()
{
	CResourceManager* Manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetPipelineStateObjectManager();
	Manager->DeleteResource(m_PsoHandle);

	Super::Finalize();
}

void CQuadPolygonMaterial::SetGeometryBuffer(ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* _Buffer)
{
	m_SceneData = DCast<FQuadPolygonSceneData*>(_Buffer);
}

uint32 CQuadPolygonMaterial::GetDrawIndex(uint32 _Index) const
{
	return 0;
}

uint32 CQuadPolygonMaterial::GetMaterialNum() const
{
	return 1;
}

ATLANTIS_NAMESPACE::CGraphicsPipeline* CQuadPolygonMaterial::GetGraphicsPipeline() const
{
	return GetPipelineStateObject()->GetPipeline();
}

ATLANTIS_NAMESPACE::CRootSignature* CQuadPolygonMaterial::GetRootSignature() const
{
    return GetPipelineStateObject()->GetRootSignature();
}

const ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* CQuadPolygonMaterial::GetGeometryBuffer() const
{
	return m_SceneData;
}

ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* CQuadPolygonMaterial::GetGeometryBufferEdit() const
{
	return m_SceneData;
}


const CPipelineStateObject* CQuadPolygonMaterial::GetPipelineStateObject() const
{
	const CResourceManager* Manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetPipelineStateObjectManager();

	return PCast<const CPipelineStateObject*>(Manager->SearchResourceRef(m_PsoHandle));
}

bool CQuadPolygonMaterial::CreateMaterials(const FQuadPolygonMaterialInitializer* _Initializer)
{
	return true;
}

bool CQuadPolygonMaterial::CreatePipelineStateObject(const FQuadPolygonMaterialInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	CResourceManager* Manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetPipelineStateObjectManager();
	CHECK_RESULT_FALSE(Manager);

	m_PsoHandle = Manager->SearchResourceHandle(_Initializer->PsoName);
	CHECK_RESULT_FALSE(Manager->IsValidHandle(m_PsoHandle));

	return true;
}

bool CQuadPolygonMaterial::CreateSceneData(const FQuadPolygonMaterialInitializer* _Initializer)
{
	return true;
}
