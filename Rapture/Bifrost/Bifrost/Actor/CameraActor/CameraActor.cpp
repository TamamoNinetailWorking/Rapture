#include "CameraActor.h"

#include <Bifrost/Component/Transform/TransformComponent.h>
#include <Bifrost/Component/Camera/CameraComponent.h>

#include <DirectXMath.h>

using namespace DirectX;

USING_ATLANTIS;

using namespace Glue;

USING_BIFROST;

CCameraActor::CCameraActor() : Super()
{
}

CCameraActor::~CCameraActor()
{
	CActor::~CActor();
}

bool CCameraActor::Initialize(const FActorInitializerBase* _Initializer)
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
			m_CameraComponent = new CCameraComponent();
			CHECK_RESULT_BREAK(m_CameraComponent);
			CHECK_RESULT_BREAK(m_CameraComponent->Initialize());

			AttachComponent(m_CameraComponent);
		}

		return true;
	} while (0);

	Finalize();
	return false;
}

void CCameraActor::Finalize()
{
	Super::Finalize();
}

void CCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

void CCameraActor::EndPlay()
{
	Super::EndPlay();
}

bool CCameraActor::Update(float _DeltaTime)
{
	return Super::Update(_DeltaTime);
}

const CCameraActor::Matrix CCameraActor::GetViewMatrix() const
{
	return m_CameraComponent->GetViewMatrix();
}

const CCameraComponent* CCameraActor::GetCameraComponent() const
{
	return m_CameraComponent;
}

CCameraComponent* CCameraActor::GetCameraComponentEdit() const
{
	return m_CameraComponent;
}
