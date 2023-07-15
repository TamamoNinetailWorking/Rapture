#include "LightActor.h"

#include <Bifrost/Component/Transform/TransformComponent.h>
#include <Bifrost/Component/Light/LightComponent.h>

USING_BIFROST;

CLightActor::CLightActor() : Super()
{
}

CLightActor::~CLightActor()
{
	CActor::~CActor();
}

bool CLightActor::Initialize(const FActorInitializerBase* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(Super::Initialize(_Initializer));

		{
			m_TransformComponent = new CTransformComponent();
			CHECK_RESULT_BREAK(m_TransformComponent);
			CHECK_RESULT_BREAK(m_TransformComponent->Initialize());

			AttachComponent(m_TransformComponent);
		}

		{
			m_LightComponent = new CLightComponent();
			CHECK_RESULT_BREAK(m_LightComponent);
			CHECK_RESULT_BREAK(m_LightComponent->Initialize());

			AttachComponent(m_LightComponent);
		}

		return true;

	} while (0);

	Finalize();
	return false;
}

void CLightActor::Finalize()
{
	Super::Finalize();
}

void CLightActor::BeginPlay()
{
	Super::BeginPlay();
}

void CLightActor::EndPlay()
{
	Super::EndPlay();
}

bool CLightActor::Update(float _DeltaTime)
{
	return Super::Update(_DeltaTime);
}

const CLightComponent* CLightActor::GetLightComponent() const
{
	return m_LightComponent;
}

CLightComponent* CLightActor::GetLightComponentEdit() const
{
	return m_LightComponent;
}
