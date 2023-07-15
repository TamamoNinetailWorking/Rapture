#include "LightComponent.h"

#include <DirectXMath.h>

#include <Bifrost/Actor/Actor.h>
#include <Bifrost/Component/Transform/TransformComponent.h>

#include <Bifrost/Subsystem/Light/LightSubsystem.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>

using namespace DirectX;

using namespace ATLANTIS_NAMESPACE::Glue;

USING_BIFROST;

CLightComponent::CLightComponent() : Super()
{
	m_UpdateGroup = EUpdateGroup::UPDATE_GROUP_POST_PHYSICS;
}

CLightComponent::~CLightComponent()
{
	CComponent::~CComponent();
}

bool CLightComponent::Initialize(const FComponentInitializerBase* _Initializer)
{
	do
	{
		Super::Initialize(_Initializer);

		CLightSubsystem* subsystem = CSubsystemServiceLocator::GetLightSubsystemEdit();
		CHECK_RESULT_BREAK(subsystem);

		m_Handle = subsystem->SetLightComponent(this);

		return true;

	} while (0);

	Finalize();
	return false;
}

void CLightComponent::Finalize()
{
	CLightSubsystem* subsystem = CSubsystemServiceLocator::GetLightSubsystemEdit();
	CHECK(subsystem);

	subsystem->DeleteComponent(m_Handle);

	Super::Finalize();
}

void CLightComponent::BeginPlay()
{
	Super::BeginPlay();
}

void CLightComponent::EndPlay()
{
	Super::EndPlay();
}

bool CLightComponent::Update(float _DeltaTime)
{
	return Super::Update(_DeltaTime);
}

void CLightComponent::SetColor(const ATLANTIS_NAMESPACE::Glue::Vector3& _Color)
{
	m_Color = _Color;
}

const ATLANTIS_NAMESPACE::Glue::Vector3 CLightComponent::GetPosition() const
{
	Vector3 pos = {};
	CTransformComponent* trans = m_Parent->SearchComponent <CTransformComponent>();
	if (!trans) { return pos; }

	//
	// LocalPositionが必要なやつは持つようにして、
	// ローカル座標とワールド座標を別で取れるようにするのがいいだろう
	//
	//
	pos = trans->GetWorldPosition();

	return pos;
}

const ATLANTIS_NAMESPACE::Glue::Vector3 CLightComponent::GetColor() const
{
	return m_Color;
}

const FLightComponentManagerHandle& CLightComponent::GetHandle() const
{
	return m_Handle;
}


