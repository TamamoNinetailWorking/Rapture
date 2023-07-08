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
#include <Bifrost/Resource/Manager/MotionResourceManager.h>
#include <rapture/Subsystem/ResourceSubsystemImpl.h>
#include <Bifrost/Subsystem/Resource/ResourceManagementSubsystemInitializer.h>

#include <Bifrost/Subsystem/Updater/UpdateProcessorSubsystem.h>
#include <Bifrost/Subsystem/UpdateInterval/UpdateIntervalSubsystem.h>
#include <Bifrost/Subsystem/UpdateInterval/UpdateIntervalSubsystemInitializer.h>
#include <Bifrost/Subsystem/Actor/ActorSubsystem.h>
#include <Bifrost/Subsystem/Camera/CameraSubsystem.h>
#include <Bifrost/Subsystem/Light/LightSubsystem.h>

#include <Subsystem/DebugSubsystemServiceLocator.h>
//#include <Subsystem/DebugWindowSubsystem.h>
#include <rapture/DebugInterface/Main/DebugWindowSubsystemImpl.h>
#include <Subsystem/DebugWindowSubsystemInitializer.h>

#include <rapture/Environment/ResourceTypeDefine.h>
#include <Bifrost/Resource/DefaultResourceDefine.h>

#include <eden/include/gadget/timer/timer.h>

// このヘッダーはSubsystemのServiceLocaterがあるから要らないかも
#include <Atlantis/RHIProccessor/RHIProcessor.h>

#include <rapture/Test/Test.h>
#include <Bifrost/Component/Transform/TransformComponent.h>

EDENS_NAMESPACE_USING;
USING_ATLANTIS;
USING_BIFROST;
USING_MAGALLANICA;

enum ETimerAttribute
{
	Execution = 0,
	Update,
	Render,

	NUM_TIMER_ATTRIBUTE,
};

CTimer g_TimerArray[NUM_TIMER_ATTRIBUTE] = {};
double g_TimerResult[NUM_TIMER_ATTRIBUTE] = {};

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
			initializer.CommandListType = ECommandListType::COMMAND_LIST_TYPE_DIRECT;
			initializer.CommandQueueFlag = ECommandQueueFlag::COMMAND_QUEUE_FLAG_NONE;
			initializer.CommandQueuePriority = ECommandQueuePriority::COMMAND_QUEUE_PRIORITY_NORMAL;
			initializer.ViewWidth = _Initializer->ViewportWidth;
			initializer.ViewHeight = _Initializer->ViewportHeight;
			initializer.WindowHandle = _Initializer->WindowHandle;
			initializer.QueueReserveNum = 200;
			initializer.RTVFormat = EDataFormat::FORMAT_R8G8B8A8_UNORM;
			initializer.RTVDimension = ERTVDimension::RTV_DIMENSION_TEXTURE2D;
			initializer.NearZ = 0.01f;
			initializer.FarZ = 10000.0f;
			initializer.FovAngle = DirectX::XM_PIDIV2;


			CHECK_RESULT_BREAK(m_RenderingSubsystem->Initialize(&initializer));

			m_SubsystemDominator->SetRenderingSubsystem(m_RenderingSubsystem);
		}

		{
			m_ResourceSubsystem = new CResourceManagementSubsystem();
			CHECK_RESULT_BREAK(m_ResourceSubsystem);

			FResourceManagementSubsystemInitializer initializer = {};
			initializer.ResourceTypeNum = UNumCast(EResourceManagementType::RESOURCE_TYPE_NUM);

			CHECK_RESULT_BREAK(m_ResourceSubsystem->Initialize(&initializer));

			
			// Texture
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CTextureResourceManager>(UNumCast(EResourceManagementType::RESOURCE_TYPE_TEXTURE)));
			}

			// Mesh
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CMeshDataManager>(UNumCast(EResourceManagementType::RESOURCE_TYPE_MESH)));
			}

			// Shader
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CShaderManager>(UNumCast(EResourceManagementType::RESOURCE_TYPE_SHADER)));
			}

			// Material
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CMaterialManager>(UNumCast(EResourceManagementType::RESOURCE_TYPE_MATERIAL)));
			}

			// PSO
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CPipelineStateObjectManager>(UNumCast(EResourceManagementType::RESOURCE_TYPE_PSO)));
			}

			// Motion
			{
				CHECK_RESULT_BREAK(m_ResourceSubsystem->SetupManager<CMotionResourceManager>(UNumCast(EResourceManagementType::RESOURCE_TYPE_MOTION)));
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

			m_SubsystemDominator->SetUpdateProcessorSubsystem(m_UpdaterSubsystem);
		}

		{
			m_UpdateIntervalSubsystem = new CUpdateIntervalSubsystem();
			CHECK_RESULT_BREAK(m_UpdateIntervalSubsystem);

			FUpdateIntervalSubsystemInitializer initializer = {};
			initializer.FramePerSecond = 60;

			CHECK_RESULT_BREAK(m_UpdateIntervalSubsystem->Initialize(&initializer));

			m_SubsystemDominator->SetUpdateIntervalSubsystem(m_UpdateIntervalSubsystem);
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

		//{
		//	// フレームレートを制限するための仕組みを作った方が良いかも

		//	// Gadget
		//	m_Timer = new CTimer();
		//	CHECK_RESULT_BREAK(m_Timer);

		//	m_Timer->SetUp();
		//}

		{
			for (auto& elem : g_TimerArray)
			{
				elem.SetUp();
			}
		}

		{
			m_DebugWindowSubsystem = new CDebugWindowSubsystemImpl();
			CHECK_RESULT_BREAK(m_DebugWindowSubsystem);

			FDebugWindowSubsystemInitializer initializer = {};
			initializer.hwnd = _Initializer->WindowHandle;
			initializer.Processor = m_RenderingSubsystem->GetProcessor();

			CHECK_RESULT_BREAK(m_DebugWindowSubsystem->Initialize(&initializer));

			CDebugSubsystemServiceLocator::SetDebugWindowSubsystem(m_DebugWindowSubsystem);
		}
		
		{
			Test::TestMain();
		}

		return true;

	}while (0);


	Finalize();
	return false;
}

void CGameManager::Finalize()
{
	FinalizeObject(m_DebugWindowSubsystem);
	//Delete(m_Timer);
	FinalizeObject(m_CameraSubsystem);
	FinalizeObject(m_LightSubsystem);
	FinalizeObject(m_ActorSubsytem);
	FinalizeObject(m_UpdateIntervalSubsystem);
	FinalizeObject(m_UpdaterSubsystem);
	FinalizeObject(m_RenderingSubsystem);
	Release(m_ResSystemInterface);
	FinalizeObject(m_ResourceSubsystem);
	FinalizeObject(m_SubsystemDominator);
}

void CGameManager::GameMain()
{
	// UpdateIntervalSubsystemを使ってフレームレート制御
	// GameMainの中身をバインドする形になるかと思われる

	//m_Timer->RecordCurrentTime();
	//
	//constexpr double FrameTime = 1.0 / 60;
	//if (m_Timer->CalculateExecuteTime() >= FrameTime)
	//{
	//	m_Timer->RecordCurrentTime();

	//	GameUpdate();
	//	Render();

	//	m_Timer->RecordStartTime();
	//}



	std::function<void(float)> ExecuteFunc = [&](float _DeltaTime)
	{
		// 処理時間を計測するために一時的に3つのタイマーを用意
		g_TimerArray[ETimerAttribute::Execution].RecordStartTime();

		g_TimerArray[ETimerAttribute::Update].RecordStartTime();
		this->GameUpdate(_DeltaTime);
		g_TimerArray[ETimerAttribute::Update].RecordCurrentTime();
		g_TimerResult[ETimerAttribute::Update] = g_TimerArray[ETimerAttribute::Update].CalculateExecuteTime();


		g_TimerArray[ETimerAttribute::Render].RecordStartTime();
		this->Render();
		g_TimerArray[ETimerAttribute::Render].RecordCurrentTime();
		g_TimerResult[ETimerAttribute::Render] = g_TimerArray[ETimerAttribute::Render].CalculateExecuteTime();

		g_TimerArray[ETimerAttribute::Execution].RecordCurrentTime();
		g_TimerResult[ETimerAttribute::Execution] = g_TimerArray[ETimerAttribute::Execution].CalculateExecuteTime();
	};

	m_UpdateIntervalSubsystem->Execution(ExecuteFunc);
}


void CGameManager::GameUpdate(float _DeltaTime)
{
	// 描画処理が終わったあととかに破壊しておきたい
	m_ActorSubsytem->DestroyActors();

	// 処理時間を入れたが、最初の初期化処理を含めたフレームの待ち時間が長すぎておかしくなるので、1フレーム目だけ一旦すっ飛ばしてみる
	static bool tempFlag = false;
	float timeDeltaTime = _DeltaTime;
	if (!tempFlag)
	{
		timeDeltaTime = 0.f;
		tempFlag = true;
	} // ※初期化処理を高速化したり、時間初期化位置を変更したりするまで暫定的措置

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
	// ↑に処理時間を入れた

	m_UpdaterSubsystem->ProcessorUpdate(timeDeltaTime);// DeltaTimeを持ってくる

	m_DebugWindowSubsystem->ShowDebugWindow(timeDeltaTime);

	m_UpdaterSubsystem->EndPlayExecute();
}

void CGameManager::Render()
{
	CHECK(m_RenderingSubsystem->RenderBegin());

	m_RenderingSubsystem->Rendering();

	m_DebugWindowSubsystem->RenderDebugWindow();

	m_RenderingSubsystem->RenderEnd();
}
