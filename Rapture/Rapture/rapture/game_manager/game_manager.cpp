#include "game_manager.h"
#include "game_manager_def.h"

#include <eden/include/utility/ender_utility.h>

#include <DirectXMath.h>
#include <Atlantis/DirectX12/GlueMath.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

#include <Bifrost/Subsystem/Dominator/SubsystemDominator.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>

#include <Bifrost/Subsystem/Resource/ResourceManagementSubsystem.h>
#include <Bifrost/Subsystem/Rendering/RenderingSubsystem.h>
#include <Bifrost/Subsystem/Rendering/RenderingSubsystemInitializer.h>
#include <Atlantis/Resource/TextureResourceManager.h>
#include <Atlantis/Resource/ShaderManager.h>
#include <Bifrost/Resource/Manager/MeshDataManager.h>
#include <Bifrost/Resource/Manager/MaterialManager.h>
#include <Bifrost/Resource/Manager/PipelineStateObjectManager.h>
#include <rapture/Subsystem/ResourceSubsystemImpl.h>
#include <Bifrost/Subsystem/Updater/UpdateProcessorSubsystem.h>
#include <Bifrost/Subsystem/Actor/ActorSubsystem.h>
#include <Bifrost/Subsystem/Camera/CameraSubsystem.h>
#include <Bifrost/Subsystem/Light/LightSubsystem.h>

#include <rapture/Environment/ResourceTypeDefine.h>

#include <Bifrost/Resource/DefaultResourceDefine.h>

#include <Atlantis/RHIProccessor/RHIProcessor.h>

#include <rapture/Test/Test.h>
#include <Bifrost/Component/Transform/TransformComponent.h>

EDENS_NAMESPACE_USING;
USING_ATLANTIS;
USING_BIFROST;


b8 CGameManager::Initialize(FGameManagerInitializer * _Initializer)
{
	do 
	{

		{
			m_SubsystemDominator = new CSubsystemDominator();
			CHECK_RESULT_BREAK(m_SubsystemDominator);

			CHECK_RESULT_BREAK(m_SubsystemDominator->Initialize());
			CSubsystemServiceLocator::SetSubsystemDominator(m_SubsystemDominator);
		}

		{

			using namespace Glue;

			m_RenderingSubsystem = new CRenderingSubsystem();
			CHECK_RESULT_BREAK(m_RenderingSubsystem);

			FRenderingSubsystemInitializer initializer = {};
			initializer.BarrierType = Glue::EResourceBarrierType::BARRIER_TYPE_TRANSITION;
			initializer.CommandListType = COMMAND_LIST_TYPE_DIRECT;
			initializer.CommandQueueFlag = COMMAND_QUEUE_FLAG_NONE;
			initializer.CommandQueuePriority = COMMAND_QUEUE_PRIORITY_NORMAL;
			initializer.ViewWidth = _Initializer->ViewportWidth;
			initializer.ViewHeight = _Initializer->ViewportHeight;
			initializer.WindowHandle = _Initializer->WindowHandle;
			initializer.QueueReserveNum = 200;
			initializer.RTVFormat = FORMAT_R8G8B8A8_UNORM;
			initializer.RTVDimension = RTV_DIMENSION_TEXTURE2D;
			initializer.NearZ = 0.01f;
			initializer.FarZ = 10000.0f;
			initializer.FovAngle = DirectX::XM_PIDIV2;


			CHECK_RESULT_BREAK(m_RenderingSubsystem->Initialize(&initializer));

			m_SubsystemDominator->SetRenderingSubsystem(m_RenderingSubsystem);
		}

		{
			m_ResourceSubsystem = new CResourceManagementSubsystem();
			CHECK_RESULT_BREAK(m_ResourceSubsystem);

			CResourceManagementSubsystem::FInitializer initializer = {};
			initializer.ResourceTypeNum = EResourceManagementType::RESOURCE_TYPE_NUM;

			CHECK_RESULT_BREAK(m_ResourceSubsystem->Initialize(initializer));

			
			// Texture
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CTextureResourceManager>(EResourceManagementType::RESOURCE_TYPE_TEXTURE));
			}

			// Mesh
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CMeshDataManager>(EResourceManagementType::RESOURCE_TYPE_MESH));
			}

			// Shader
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CShaderManager>(EResourceManagementType::RESOURCE_TYPE_SHADER));
			}

			// Material
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CMaterialManager>(EResourceManagementType::RESOURCE_TYPE_MATERIAL));
			}

			// PSO
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CPipelineStateObjectManager>(EResourceManagementType::RESOURCE_TYPE_PSO));
			}

			m_ResSystemInterface = new CResourceSubsystemImpl();
			CHECK_RESULT_BREAK(m_ResSystemInterface);
			auto ptr = PCast<CResourceSubsystemImpl*>(m_ResSystemInterface);
			
			ptr->SetSubsystem(m_ResourceSubsystem);


			m_SubsystemDominator->SetResourceSubsystem(ptr);

			auto* mainDevice = m_RenderingSubsystem->GetProcessorEdit()->GetDeviceEdit();

			CHECK_RESULT_BREAK(ptr->CreateDefaultTextureResource(mainDevice));

			// DefaultPSO Create
			{
				// Pmd
				{
					FShaderNameBlock nameBlock = {};
					nameBlock.VS.File = "resource/cso/PmdVertexShader.cso";
					nameBlock.VS.Function = "main";
					nameBlock.PS.File = "resource/cso/PmdPixelShader.cso";
					nameBlock.PS.Function = "main";

					CHECK_RESULT_BREAK(ptr->CreatePmdPipelineStateObject(mainDevice, nameBlock));
				}
			}
		}

		{
			m_UpdaterSubsystem = new CUpdateProcessorSubsystem();
			CHECK_RESULT_BREAK(m_UpdaterSubsystem);
			
			CHECK_RESULT_BREAK(m_UpdaterSubsystem->Initialize());

			m_SubsystemDominator->SetUpdaterSubsystem(m_UpdaterSubsystem);
		}

		{
			m_ActorSubsytem = new CActorSubsystem();
			CHECK_RESULT_BREAK(m_ActorSubsytem);

			CHECK_RESULT_BREAK(m_ActorSubsytem->Initialize());

			m_SubsystemDominator->SetActorSubsystem(m_ActorSubsytem);
		}

		{
			m_CameraSubsystem = new CCameraSubsystem();
			CHECK_RESULT_BREAK(m_CameraSubsystem);

			CHECK_RESULT_BREAK(m_CameraSubsystem->Initialize());

			m_SubsystemDominator->SetCameraSubsystem(m_CameraSubsystem);
		}

		{
			m_LightSubsystem = new CLightSubsystem();
			CHECK_RESULT_BREAK(m_LightSubsystem);

			CHECK_RESULT_BREAK(m_LightSubsystem->Initialize());

			m_SubsystemDominator->SetLightSubsystem(m_LightSubsystem);
		}
		
		{
			auto* mainDevice = m_RenderingSubsystem->GetProcessorEdit()->GetDeviceEdit();

			Test::MatrixVectorTestMain();

			Test::FileLoaderTest(mainDevice);
			Test::GraphicsPipelineTest(mainDevice);
			Test::BindFunctionTest();

			Test::RenderSubsystemTest();
		}

		return true;

	}while (0);


	Finalize();
	return false;
}

void CGameManager::Finalize()
{
	FinalizeObject(m_CameraSubsystem);
	FinalizeObject(m_LightSubsystem);
	FinalizeObject(m_ActorSubsytem);
	FinalizeObject(m_UpdaterSubsystem);
	FinalizeObject(m_RenderingSubsystem);
	Release(m_ResSystemInterface);
	FinalizeObject(m_ResourceSubsystem);
	FinalizeObject(m_SubsystemDominator);
}

void CGameManager::GameMain()
{
	GameUpdate();
	Render();
}


void CGameManager::GameUpdate()
{
	// 描画処理が終わったあととかに破壊しておきたい
	m_ActorSubsytem->DestroyActors();

	m_UpdaterSubsystem->BeginPlayExecute();

	using namespace DirectX;

	using namespace Glue;

	static float rotSpeed = 0.f;

	float yaw = (XM_PI / 180.f) * rotSpeed;

	XMMATRIX pose = XMMatrixIdentity();
	XMMATRIX trans = XMMatrixIdentity();
	XMMATRIX scale = XMMatrixIdentity();
	pose = XMMatrixRotationY(yaw);

	//-----------
	// ActorManagerに生成処理を入れて、Actor処理が走るようになれば概ね形になるはず
	//-----------
	Test::m_TransformComponent->SetRotate(Vector3(0.f, yaw, 0.f));

	rotSpeed += 1.0f;
	if (rotSpeed > 360.f)
	{
		rotSpeed = 0.f;
	}
	// 処理時間の制限を行う場所を、AppManagerからここに移動させてもいいかもしれない
	// >> そもそも処理時間を今制限していない問題
	m_UpdaterSubsystem->ProcessorUpdate(0.f);// DeltaTimeを持ってくる


	m_UpdaterSubsystem->EndPlayExecute();
}

void CGameManager::Render()
{
	m_RenderingSubsystem->Rendering();
}
