#include "Test.h"
#include <Bifrost/Component/Pmd/PmdModelComponent.h>
#include <Bifrost/Component/Pmd/PmdModelComponentInitializer.h>

#include <Bifrost/Component/Transform/TransformComponent.h>

#include <rapture/Environment/PsoNameDefine.h>

#include <Bifrost/Actor/Actor.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Actor/ActorSubsystem.h>

#include <Bifrost/Actor/CameraActor/CameraActor.h>

#include <Bifrost/Component/ComponentManager.h>

#include <Bifrost/Component/Camera/CameraComponent.h>
#include <Bifrost/Component/Camera/CameraComponentManager.h>
#include <Bifrost/Actor/CameraActor/CameraActor.h>
#include <Bifrost/Subsystem/Camera/CameraSubsystem.h>

#include <Bifrost/Component/Light/LightComponent.h>
#include <Bifrost/Actor/LightActor/LightActor.h>
#include <Bifrost/Subsystem/Light/LightSubsystem.h>

USING_BIFROST;

USING_ATLANTIS;

namespace Test
{
	CTransformComponent* m_TransformComponent = nullptr;
	CPmdModelComponent* m_PmdModelComponent = nullptr;
	//CLightComponent* m_LightComponent = nullptr;
	//CCameraComponent* m_CameraComponent = nullptr;

	CLightActor* m_LightActor = nullptr;
	CCameraActor* m_CameraActor = nullptr;

	CActor* m_TestActor = nullptr;

#if 0
	TComponentManager<CTransformComponent> m_Manager = {};

	CCameraComponent* TestCameraComponent = nullptr;
	CCameraComponentManager CameraManager = {};
#endif

	void RenderSubsystemTest()
	{
		{
			m_PmdModelComponent = new CPmdModelComponent();
			CHECK(m_PmdModelComponent);

			const char* fileName = "resource/mmd/UserFile/Model/初音ミクmetal.pmd";

			FPmdModelComponentInitializer initializer = {};
			initializer.FileName = CHash160(fileName);
			initializer.ToonMapDirectory = CHash160("resource/mmd/Data/");
			initializer.PsoName = PsoName::PmdRenderingPso;

			if (!m_PmdModelComponent->Initialize(&initializer))
			{
				PRINT("PmdModelComponent is not created.\n");
				return;
			}

			//m_PmdModelComponent->BeginPlay();
		}

		{
			m_TransformComponent = new CTransformComponent;
			CHECK(m_TransformComponent);

			FComponentInitializerBase* initializer = nullptr;

			if (!m_TransformComponent->Initialize(initializer))
			{
				PRINT("TransformComponent is not created.\n");
				return;
			}
#if 0
			// ComponentManagerTest
			{
				m_Manager.Initialize();

				FComponentManagerHandle<CTransformComponent> handle = {};
				handle = m_Manager.RegistComponent(m_TransformComponent);

				m_Manager.DeleteComponent(handle);

				auto* temp = m_Manager.SearchComponent(handle);


				m_Manager.Finalize();
			}
#endif
		}

		{
			CActorSubsystem* subsystem = CSubsystemServiceLocator::GetActorSubsystemEdit();
			CHECK(subsystem);

			FActorInitializerBase* initializer = nullptr;

			m_TestActor = subsystem->CreateActor<CActor>(initializer);

			m_TestActor->SetHash(CHash160("TestActor01"));

			m_TestActor->AttachComponent(m_PmdModelComponent);
			m_TestActor->AttachComponent(m_TransformComponent);
		}

		{
			CActorSubsystem* subsystem = CSubsystemServiceLocator::GetActorSubsystemEdit();
			CHECK(subsystem);

			m_CameraActor = new CCameraActor();
			CHECK(m_CameraActor);

			m_CameraActor = subsystem->CreateActor<CCameraActor>();

			CTransformComponent* trans = m_CameraActor->SearchComponent<CTransformComponent>();
			CHECK(trans);

			trans->SetWorldPosition(Glue::Vector3(0.f, 13.5f, -15.f));

			CCameraSubsystem* cameraSubsystem = CSubsystemServiceLocator::GetCameraSubsystemEdit();
			CHECK(cameraSubsystem);

			cameraSubsystem->SetMainCameraActor(m_CameraActor);
		}

		{
			CActorSubsystem* subsystem = CSubsystemServiceLocator::GetActorSubsystemEdit();
			CHECK(subsystem);

			m_LightActor = new CLightActor();
			CHECK(m_LightActor);

			m_LightActor = subsystem->CreateActor<CLightActor>();

			CTransformComponent* trans = m_LightActor->SearchComponent<CTransformComponent>();
			CHECK(trans);

			trans->SetWorldPosition(Glue::Vector3(5.f, 20.f, -8.f));

			CLightComponent* light = m_LightActor->SearchComponent<CLightComponent>();
			CHECK(light);

			light->SetColor(Glue::Vector3(1.f, 1.f, 1.f));

			CLightSubsystem* lightSubsystem = CSubsystemServiceLocator::GetLightSubsystemEdit();
			CHECK(lightSubsystem);

			lightSubsystem->SetMainLightComponent(light);

		}

#if 0



		CCameraActor* cameraActor = nullptr;

		{
			CActorSubsystem* subsystem = CSubsystemServiceLocator::GetActorSubsystemEdit();
			CHECK(subsystem);

			cameraActor = subsystem->CreateActor<CCameraActor>(nullptr);

			auto mat = cameraActor->GetViewMatrix();
		}

		{
			TestCameraComponent = new CCameraComponent();
			CHECK(TestCameraComponent);

			CHECK(TestCameraComponent->Initialize());

			CameraManager.Initialize();

			CCameraComponentManager::Handle handle = {};

			handle = CameraManager.RegistComponent(TestCameraComponent);

			CameraManager.Finalize();


			//こういうワンフレームでObjectの生成破棄ができない
			// >> 必要があるかは別にして、安全性のためにはOnceExecuterにもハンドルを用意し、Finalize時にはExecuterから外す処理が必要だろう
			// 
			// そこに付随して、Subsystem周りはHandleも含めてTemplate化するのがいいかもしれない
			// >>ComponentManagerがいいTemplate化の見本かも
			// 
			//TestCameraComponent->Finalize();
			//delete TestCameraComponent;
			//TestCameraComponent = nullptr;
		}
#endif
	}

};