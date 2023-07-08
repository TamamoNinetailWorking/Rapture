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

#include <Atlantis/DirectX12/GlueMath.h>

#include <rapture/Environment/PsoNameDefine.h>

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
				const char* fileName = "resource/mmd/UserFile/Model/èââπÉ~ÉNmetal.pmd";

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

			CLightActor* lightActor = new CLightActor();
			CHECK_RESULT_BREAK(lightActor);

			lightActor = SpawnActor<CLightActor>(CHash160("Light01"));

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
