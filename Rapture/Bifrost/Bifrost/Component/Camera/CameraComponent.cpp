#include "CameraComponent.h"

#include <DirectXMath.h>

#include <Bifrost/Actor/Actor.h>
#include <Bifrost/Component/Transform/TransformComponent.h>

#include <Bifrost/Subsystem/Camera/CameraSubsystem.h>
#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>

using namespace DirectX;

using namespace ATLANTIS_NAMESPACE::Glue;

USING_BIFROST;

CCameraComponent::CCameraComponent() : Super()
{
	m_UpdateGroup = EUpdateGroup::UPDATE_GROUP_POST_PHYSICS;
	//m_CanEverUpdate = false;
}

CCameraComponent::~CCameraComponent()
{
	CComponent::~CComponent();
}

bool CCameraComponent::Initialize(const FComponentInitializerBase* _Initializer)
{
	do
	{
		Super::Initialize(_Initializer);

		CCameraSubsystem* subsystem = CSubsystemServiceLocator::GetCameraSubsystemEdit();
		CHECK_RESULT_BREAK(subsystem);

		m_Handle = subsystem->SetCameraComponent(this);

		return true;

	} while (0);

	Finalize();
	return false;
}

void CCameraComponent::Finalize()
{
	CCameraSubsystem* subsystem = CSubsystemServiceLocator::GetCameraSubsystemEdit();
	CHECK(subsystem);

	subsystem->DeleteComponent(m_Handle);

	Super::Finalize();
}

void CCameraComponent::BeginPlay()
{
	Super::BeginPlay();
}

void CCameraComponent::EndPlay()
{
	Super::EndPlay();
}

bool CCameraComponent::Update(float _DeltaTime)
{
	return Super::Update(_DeltaTime);
}

const ATLANTIS_NAMESPACE::Glue::FMatrix CCameraComponent::GetViewMatrix() const
{
	FMatrix matrix = {};
	if (!m_Parent) { return matrix; }

	CTransformComponent* trans = m_Parent->SearchComponent<CTransformComponent>();
	if (!trans) { return matrix; }

	FMatrix transMatrix = trans->GetTransformMatrix();

	transMatrix.r[3] = XMVectorSet(0.f,0.f,0.f,0.f);

	transMatrix = XMMatrixTranspose(transMatrix);

	Vector3 transPos = trans->GetWorldPosition();
	FVector position = {};
	position = XMVectorSet(-transPos.x, -transPos.y, -transPos.z, 1.0f);

	transMatrix.r[3] = position;

	return transMatrix;
}

const ATLANTIS_NAMESPACE::Glue::Vector3 CCameraComponent::GetPosition() const
{
	Vector3 position = {};
	CTransformComponent* trans = m_Parent->SearchComponent<CTransformComponent>();
	if (!trans) { return position; }
	position = trans->GetWorldPosition();
	return position;
}

const FCameraComponentManagerHandle& CCameraComponent::GetHandle() const
{
	// TODO: return ステートメントをここに挿入します
	return m_Handle;
}
