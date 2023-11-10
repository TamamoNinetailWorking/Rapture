#include "ExperimentationLevel.h"

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Camera/CameraSubsystem.h>
#include <Bifrost/Subsystem/Light/LightSubsystem.h>

#include <Bifrost/Actor/Actor.h>

#include <Bifrost/Actor/CameraActor/CameraActor.h>

#include <Bifrost/Actor/LightActor/LightActor.h>
#include <Bifrost/Component/Light/LightComponent.h>

#include <Bifrost/Component/Pmd/PmdModelComponent.h>
#include <Bifrost/Component/Pmd/PmdModelComponentInitializer.h>

#include <Bifrost/Component/Transform/TransformComponent.h>

#include <Atlantis/Math/GlueMath.h>

#include <rapture/Environment/PsoNameDefine.h>

// ↓テスト用
#if 0
#include <Bifrost/Subsystem/Rendering/RenderingSubsystem.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>
#include <eden/include/Resource/ResourceManager.h>
#include <Atlantis/RHIProccessor/RHIProcessor.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetView.h>
#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>
#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/RHIProccessor/RHIRenderTargetView.h>
#include <Atlantis/DirectX12/SwapChain/SwapChain.h>
#include <Atlantis/DirectX12/Texture/TextureResourceInitializer.h>
#include <Atlantis/DirectX12/Texture/TextureResource.h>

#include <Atlantis/DirectX12/RenderTargetView/RenderTargetViewResource.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetViewResourceInitializer.h>
#include <Atlantis/RHIProccessor/RHIRenderTargetDefine.h>
#include <Bifrost/RenderTarget/RenderTarget.h>
#include <Bifrost/RenderTarget/RenderTargetInitializer.h>
#endif

#include <Bifrost/Component/Rendering/PostEffect/BloomPostEffectComponent.h>
#include <Bifrost/Actor/PostEffecctActor/PostEffectActorInitializer.h>
#include <Bifrost/Actor/PostEffecctActor/PostEffectActor.h>


USING_ATLANTIS;
USING_BIFROST;

bool CExperimentationLevel::Initialize()
{
	do
	{
		//Model
		{
			CPmdModelComponent* model = nullptr;
			CTransformComponent* transform = nullptr;

			{
				const char* fileName = "resource/mmd/UserFile/Model/初音ミクmetal.pmd";

				FPmdModelComponentInitializer initializer = {};
				initializer.FileName = CHash160(fileName);
				initializer.ToonMapDirectory = CHash160("resource/mmd/Data/");
				initializer.PsoName = PsoName::PmdRenderingPso;

				model = CreateComponent<CPmdModelComponent>(&initializer);
				CHECK_RESULT_BREAK(model);
			}

			{
				transform = CreateComponent<CTransformComponent>();
				CHECK_RESULT_BREAK(transform);
			}

			CActor* modelActor = SpawnActor<CActor>(CHash160("PmdModel"));
			CHECK_RESULT_BREAK(modelActor);

			modelActor->AttachComponent(model);
			modelActor->AttachComponent(transform);

		}

		// Camera
		{
			CCameraActor* cameraActor = SpawnActor<CCameraActor>(CHash160("Camera01"));

			CTransformComponent* trans = cameraActor->SearchComponent<CTransformComponent>();
			CHECK_RESULT_BREAK(trans);

			trans->SetWorldPosition(Glue::Vector3(0.f, 13.5f, -15.f));

			CCameraSubsystem* cameraSubsystem = CSubsystemServiceLocator::GetCameraSubsystemEdit();
			CHECK_RESULT_BREAK(cameraSubsystem);

			cameraSubsystem->SetMainCameraActor(cameraActor);
		}

		// Light
		{
			CLightActor* lightActor = SpawnActor<CLightActor>(CHash160("Light01"));

			CTransformComponent* trans = lightActor->SearchComponent<CTransformComponent>();
			CHECK_RESULT_BREAK(trans);

			trans->SetWorldPosition(Glue::Vector3(5.f, 20.f, -8.f));

			CLightComponent* light = lightActor->SearchComponent<CLightComponent>();
			CHECK_RESULT_BREAK(light);

			light->SetColor(Glue::Vector3(1.f, 1.f, 1.f));

			CLightSubsystem* lightSubsystem = CSubsystemServiceLocator::GetLightSubsystemEdit();
			CHECK_RESULT_BREAK(lightSubsystem);

			lightSubsystem->SetMainLightActor(lightActor);
		}

#if 0
		// レンダーターゲットの作成テスト
		{
			CRenderingSubsystem* Subsystem = CSubsystemServiceLocator::GetRenderingSubsystemEdit();
			CHECK_RESULT_BREAK(Subsystem);

			IResourceSubsystem::ManagerPtr Manager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetTextureResourceManagerEdit();
			CHECK_RESULT_BREAK(Manager);

			IResourceSubsystem::ManagerPtr RTVManager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetRenderTargetViewResourceManagerEdit();
			CHECK_RESULT_BREAK(RTVManager);

			EDENS_NAMESPACE::FResourceHandle handle = {};

			D3D12_RESOURCE_DESC desc = Subsystem->GetRTV()->GetSwapChain()->GetBackBuffer()->at(0)->GetDesc();

			{
				FTextureInitializer initializer = {};
				initializer.Device = Subsystem->GetProcessor()->GetDevice()->GetDevice();
				initializer.Name = CHash160("RenderTarget01");
				initializer.HeapType = Glue::EHeapType::DEFAULT;
				initializer.pResDesc = &desc;
				D3D12_CLEAR_VALUE ClearValue = CD3DX12_CLEAR_VALUE(Glue::GetDXGIFormat(Glue::EDataFormat::FORMAT_R8G8B8A8_UNORM), RenderTargetClearColor);
				//initializer.ClearValue = &ClearValue;
				initializer.ClearValue = RenderTargetClearColor;

				handle = Manager->SearchCreateResource(&initializer);
				CHECK_RESULT_BREAK(Manager->IsValidHandle(handle));
			}

			const CTextureResource* TexRes = nullptr;
			TexRes = SCast<const CTextureResource*>(Manager->SearchResourceRef(handle));

#if 0
			CRenderTargetView* RenderTargetView = new CRenderTargetView();
			{
				CRenderTargetView::FRenderTargetViewInitializer initializer = {};
				initializer.Device = Subsystem->GetProcessor()->GetDevice()->GetDevice();
				initializer.ResPtr = TexRes->GetResource();
				initializer.Format = SCast<Glue::EDataFormat>(desc.Format);
				initializer.RTVDimension = Glue::ERTVDimension::RTV_DIMENSION_TEXTURE2D;
					
				CHECK_RESULT_BREAK(RenderTargetView->Initialize(initializer));
			}
#endif
#if 1
			{
				EDENS_NAMESPACE::FResourceHandle rtvHandle = {};
				FRenderTargetViewResourceInitializer initializer = {};
				initializer.Name = CHash160("RenderTargetRes01");
				initializer.Device = Subsystem->GetProcessor()->GetDevice()->GetDevice();
				initializer.ResPtr = TexRes->GetResource();
				initializer.Format = SCast<Glue::EDataFormat>(desc.Format);
				initializer.RTVDimension = Glue::ERTVDimension::RTV_DIMENSION_TEXTURE2D;

				rtvHandle = RTVManager->SearchCreateResource(&initializer);
				CHECK_RESULT_BREAK(RTVManager->IsValidHandle(rtvHandle));
			}
#endif
		}

		{
			FRenderTargetInitializer initializer = {};
			initializer.Name = CHash160("RenderTargetTest01");
			initializer.ClearValue = RenderTargetClearColor;

			CRenderTarget* RenderTarget = new CRenderTarget();
			CHECK_RESULT_BREAK(RenderTarget->Initialize(&initializer));
		}
#endif

#if 0
		// ポストエフェクトアクターの作成
		{
			FPostEffectActorInitializer initializer = {};
			initializer.QuadPolygonPsoName = PsoName::QuadPolygonRenderingPso;
			initializer.BrightnessPsoName = PsoName::BrightnessThresholdPso;
			initializer.BlurPsoName = PsoName::BlurRenderingPso;
			initializer.FetchColorPsoName = PsoName::FetchColorPso;

			CPostEffectActor* PostEffectActor = SpawnActor<CPostEffectActor>(CHash160("PostEffectActor01"),&initializer);
		}
#endif

		PRINT("ExperimentationLevel Start\n");

		return true;
	}
	while (0);

	Finalize();
	return false;
}

void CExperimentationLevel::Finalize()
{
	Super::Finalize();
}

void CExperimentationLevel::LevelUpdate(float _DeltaTime)
{



}
