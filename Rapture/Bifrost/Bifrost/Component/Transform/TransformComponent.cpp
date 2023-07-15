#include "TransformComponent.h"

#include <Atlantis/DirectX12/GlueMath.h>
#include <DirectXMath.h>

using namespace DirectX;

USING_BIFROST;

CTransformComponent::CTransformComponent() : CComponent()
{
	m_UpdateGroup = EUpdateGroup::UPDATE_GROUP_PHYSICS_DURING;
}

CTransformComponent::~CTransformComponent()
{
	CComponent::~CComponent();
}

bool CTransformComponent::Initialize(const FComponentInitializerBase* _Initializer)
{
	CHECK_RESULT_FALSE(Super::Initialize(_Initializer));
	return true;
}

void CTransformComponent::Finalize()
{
	Super::Finalize();
}

void CTransformComponent::BeginPlay()
{
	Super::BeginPlay();
}

void CTransformComponent::EndPlay()
{
	Super::EndPlay();
}

bool CTransformComponent::Update(float _DeltaTime)
{
	return Super::Update(_DeltaTime);
}

void CTransformComponent::SetWorldPosition(const Vector3& _Position)
{
	m_Transform.Position = _Position;
}

void CTransformComponent::SetRotate(const Vector3& _Rotate)
{
	m_Transform.Rotate = _Rotate;
}

void CTransformComponent::SetScale(const Vector3& _Scale)
{
	m_Transform.Scale = _Scale;
}

const FTransform& CTransformComponent::GetTransform() const
{
	return m_Transform;
}

const CTransformComponent::Vector3& CTransformComponent::GetWorldPosition() const
{
	return m_Transform.Position;
}

const CTransformComponent::Vector3& CTransformComponent::GetRotate() const
{
	return m_Transform.Rotate;
}

const CTransformComponent::Vector3& CTransformComponent::GetScale() const
{
	return m_Transform.Scale;
}

const CTransformComponent::Matrix CTransformComponent::GetTransformMatrix() const
{
	Matrix mat = {};

	Matrix trans = {};
	Matrix pose = {};
	Matrix scale = {};
	
	{
		Vector3 transVec = GetWorldPosition();
		trans = XMMatrixTranslation(transVec.x, transVec.y, transVec.z);
	}

	{
		Vector3 rotateVec = GetRotate();
		pose = XMMatrixRotationRollPitchYaw(rotateVec.x, rotateVec.y, rotateVec.z);
	}

	{
		Vector3 scaleVec = GetScale();
		scale = XMMatrixScaling(scaleVec.x, scaleVec.y, scaleVec.z);
	}

	mat = scale * pose * trans;

	return mat;
}

