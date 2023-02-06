#include "Test.h"
#include <Bifrost/Component/Pmd/PmdModelComponent.h>
#include <Bifrost/Component/Pmd/PmdModelComponentInitializer.h>

#include <Bifrost/Component/Transform/TransformComponent.h>

#include <rapture/Environment/PsoNameDefine.h>

#include <Bifrost/Actor/Actor.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Actor/ActorSubsystem.h>

USING_BIFROST;

namespace Test
{
	CTransformComponent* m_TransformComponent = nullptr;
	CPmdModelComponent* m_PmdModelComponent = nullptr;

	CActor* m_TestActor = nullptr;

	void RenderSubsystemTest()
	{
		{
			m_PmdModelComponent = new CPmdModelComponent();
			CHECK(m_PmdModelComponent);

			const char* fileName = "resource/mmd/UserFile/Model/‰‰¹ƒ~ƒNmetal.pmd";

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
	}

};