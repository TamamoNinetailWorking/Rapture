#include "PmdModelComponent.h"
#include "PmdModelComponentInitializer.h"

#include <eden/include/utility/ender_utility.h>

#include <eden/include/gadget/FileLoader/FileLoader.h>
#include <Bifrost/Model/Pmd/PmdParser.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>
#include <Bifrost/Resource/Manager/MeshDataManager.h>

#include <Bifrost/Subsystem/Rendering/RenderingSubsystem.h>
#include <Atlantis/RHIProccessor/RHIProcessor.h>

#include <Bifrost/Model/MeshData/MeshDataInitializer.h>
#include <Bifrost/Model/MeshData/MeshData.h>
#include <Bifrost/Model/Pmd/PmdMaterialDataInitializer.h>
#include <Bifrost/Resource/PSO/PipelineStateObjectInitializer.h>
#include <Bifrost/Model/Pmd/PmdMaterialData.h>
#include <Bifrost/Model/Pmd/PmdSkeleton.h>
#include <Bifrost/Model/Pmd/PmdSkeletonInitializer.h>

#include <Bifrost/Model/Pmd/PmdMaterialDefine.h>

#include <Bifrost/Actor/Actor.h>
#include <Bifrost/Component/Transform/TransformComponent.h>

#include <Bifrost/Component/Camera/CameraComponent.h>
#include <Bifrost/Component/Light/LightComponent.h>

#include <Bifrost/Subsystem/Camera/CameraSubsystem.h>
#include <Bifrost/Subsystem/Light/LightSubsystem.h>
#include <Atlantis/SceneView/SceneView.h>

USING_BIFROST;
EDENS_NAMESPACE_USING;

CPmdModelComponent::CPmdModelComponent() : Super()
{
	m_RenderGroup = ERenderGroup::RENDER_GROUP_DEFAULT;
	m_CanEverUpdate = true;
}

CPmdModelComponent::~CPmdModelComponent()
{
	Super::~CRenderingComponent();
}

bool CPmdModelComponent::Initialize(const FComponentInitializerBase* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(Super::Initialize(_Initializer));

		const FPmdModelComponentInitializer* initializer = PCast<const FPmdModelComponentInitializer*>(_Initializer);
		CHECK_RESULT_BREAK(initializer);

		CHECK_RESULT_BREAK(ReadyPmdData(initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CPmdModelComponent::Finalize()
{
	FinalizeSkeleton();
	FinalizeMaterial();
	FinalizeMesh();
	Super::Finalize();
}

void CPmdModelComponent::BeginPlay()
{
	Super::BeginPlay();
	PRINT("Called BeginPlay PmdModelComponent\n");
}

void CPmdModelComponent::EndPlay()
{
	Super::EndPlay();
}

bool CPmdModelComponent::Update(float _DeltaTime)
{
	bool result = Super::Update(_DeltaTime);

	result &= SetTransform();

	return result;
}

void CPmdModelComponent::FinalizeSkeleton()
{
	FinalizeObject(m_Skeleton);
}

void CPmdModelComponent::FinalizeMesh()
{
	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetMeshResourceManagerEdit();
	CHECK(manager);

	CHECK(m_MeshData);

	manager->DeleteResource(m_MeshData->GetName());
	m_MeshData = nullptr;
}

void CPmdModelComponent::FinalizeMaterial()
{
	FinalizeObject(m_MaterialInterface);
}

bool CPmdModelComponent::ReadyPmdData(const FPmdModelComponentInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	bool existMesh = false;
	if (SearchMeshData(_Initializer))
	{
		existMesh = true;
	}

	CHECK_RESULT_FALSE(FileLoad(_Initializer));

	if (!existMesh)
	{
		CHECK_RESULT_FALSE(CreateMeshData(_Initializer));
	}

	CHECK_RESULT_FALSE(CreateMaterial(_Initializer));

	CHECK_RESULT_FALSE(CreateSkeleton(_Initializer));

	CloseFile();

	return true;
}

bool CPmdModelComponent::FileLoad(const FPmdModelComponentInitializer* _Initializer)
{
	m_FileLoader = new CFileLoader();
	CHECK_RESULT_FALSE(m_FileLoader);

	CHECK_RESULT_FALSE(m_FileLoader->FileLoad(_Initializer->FileName));

	m_PmdParser = new CPmdParser();
	CHECK_RESULT_FALSE(m_PmdParser);

	CHECK_RESULT_FALSE(m_PmdParser->ParseData(m_FileLoader->GetData(), m_FileLoader->GetSize()));

	return true;
}

bool CPmdModelComponent::SearchMeshData(const FPmdModelComponentInitializer* _Initializer)
{
	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetMeshResourceManagerEdit();
	CHECK_RESULT_FALSE(manager);

	const CResource* resource = manager->SearchResource(_Initializer->FileName);

	if (resource == nullptr)
	{
		return false;
	}

	m_MeshData = PCast<const CMeshData*>(resource);

	return true;
}

bool CPmdModelComponent::CreateMeshData(const FPmdModelComponentInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(m_PmdParser);

	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetMeshResourceManagerEdit();
	CHECK_RESULT_FALSE(manager);

	ATLANTIS_NAMESPACE::CRHIProcessor* processor = CSubsystemServiceLocator::GetRenderingSubsystemEdit()->GetProcessorEdit();
	CHECK_RESULT_FALSE(processor);

	FMeshResourceInitializer initializer = {};
	initializer.Device = processor->GetDeviceEdit();
	initializer.Name = _Initializer->FileName;
	initializer.Vertices = m_PmdParser->GetVertices();
	initializer.VerticesSize = m_PmdParser->GetVertexNum() * m_PmdParser->GetVertexSize();
	initializer.StrideBytes = m_PmdParser->GetVertexSize();
	initializer.Indices = m_PmdParser->GetIndicies();
	initializer.IndicesSize = SCast<uint32>(sizeof(uint16)) * m_PmdParser->GetIndexNum();

	FResourceHandle handle = manager->SearchCreateResource(&initializer);

	if (manager->IsInvalidHandle(handle))
	{
		return false;
	}
	
	m_MeshData = PCast<const CMeshData*>(manager->SearchResource(handle));

	return true;
}

//bool CPmdModelComponent::SearchMaterial(const FPmdModelComponentInitializer* _Initializer)
//{
//	CHECK_RESULT_FALSE(_Initializer);
//
//
//
//	return false;
//}

bool CPmdModelComponent::CreateMaterial(const FPmdModelComponentInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(m_PmdParser);

	m_MaterialInterface = new CPmdMaterialData();
	CHECK_RESULT_FALSE(m_MaterialInterface);

	ATLANTIS_NAMESPACE::CRHIProcessor* processor = CSubsystemServiceLocator::GetRenderingSubsystemEdit()->GetProcessorEdit();
	CHECK_RESULT_FALSE(processor);

	FPmdMaterialInitializer initializer = {};
	initializer.Device = processor->GetDeviceEdit();
	initializer.Name = _Initializer->FileName;
	initializer.ToonMapDirectory = _Initializer->ToonMapDirectory;
	initializer.Materials = m_PmdParser->GetMaterials();
	initializer.MaterialNum = m_PmdParser->GetMaterialNum();

	FPipelineStateObjectInitializer psoInit = {};
	psoInit.Name = _Initializer->PsoName;
	
	initializer.PsoInit = &psoInit;

	return m_MaterialInterface->Initialize(&initializer);
}

bool CPmdModelComponent::CreateSkeleton(const FPmdModelComponentInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	CHECK_RESULT_FALSE(m_PmdParser);

	m_Skeleton = new CPmdSkeleton();
	CHECK_RESULT_FALSE(m_Skeleton);

	FPmdSkeletonInitializer initializer = {};
	initializer.Bones = m_PmdParser->GetBones();
	initializer.BoneNum = m_PmdParser->GetBoneNum();

	return m_Skeleton->Initialize(&initializer);
}

void CPmdModelComponent::CloseFile()
{
	if (m_PmdParser)
	{
		m_PmdParser->Reset();
		Delete(m_PmdParser);
	}

	if (m_FileLoader)
	{
		m_FileLoader->ResetData();
		Delete(m_FileLoader);
	}
}

bool CPmdModelComponent::SetTransform()
{
	CHECK_RESULT_FALSE(m_Parent);

	CTransformComponent* trans = m_Parent->SearchComponent<CTransformComponent>();
	CHECK_RESULT_FALSE(trans);

	const CRenderingSubsystem* rhiSubsystem = CSubsystemServiceLocator::GetRenderingSubsystem();
	CHECK_RESULT_FALSE(rhiSubsystem);
	CHECK_RESULT_FALSE(rhiSubsystem->GetSceneView());
	auto& projection = rhiSubsystem->GetSceneView()->GetProjectionMatrix();

	const CCameraSubsystem* cameraSubsystem = CSubsystemServiceLocator::GetCameraSubsystem();
	CHECK_RESULT_FALSE(cameraSubsystem);

	const CLightSubsystem* lightSubsystem = CSubsystemServiceLocator::GetLightSubsystem();
	CHECK_RESULT_FALSE(lightSubsystem);

	const auto* camera = cameraSubsystem->GetMainCameraComponent();
	CHECK_RESULT_FALSE(camera);

	const auto* light = lightSubsystem->GetMainLightComponent();
	CHECK_RESULT_FALSE(light);

	auto sceneData = GetMaterialInterface()->GetGeometryBufferEdit();
	auto& data = *(PCast<FSceneData*>(sceneData)->pData);

	data.World = trans->GetTransformMatrix();
	data.View = camera->GetViewMatrix();
	data.ViewProjection = data.View * projection;
	data.WorldViewProjection = data.World * data.ViewProjection;

	data.EyePosition = camera->GetPosition();
	data.LightPosition = light->GetPosition();
	data.LightColor = light->GetColor();

	CHECK_RESULT_FALSE(m_Skeleton);
	auto startBone = m_Skeleton->GetBoneMatrices()->data();
	auto boneMatSize = m_Skeleton->GetBoneMatricesSize();
	memcpy_s(data.BoneMatrices, BoneMatricesSize, startBone, boneMatSize);

	//static uint32 counter = 0;
	//PRINT("Pmd Model Transform. %u \n",++counter);

	return true;
}
