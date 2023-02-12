#pragma once

#include <Bifrost/Actor/Actor.h>
#include <Atlantis/DirectX12/GlueMath.h>

BIFROST_NAMESPACE_BEGIN

struct FActorInitializerBase;

class CTransformComponent;
class CCameraComponent;

class CCameraActor : public CActor
{
public:

	virtual bool Initialize(const FActorInitializerBase* _Initializer = nullptr) override;
	virtual void Finalize() override;

	virtual void BeginPlay() override;
	virtual void EndPlay() override;

	virtual bool Update(float _DeltaTime) override;

	using Matrix = ATLANTIS_NAMESPACE::Glue::FMatrix;
	const Matrix GetViewMatrix() const;

	const CCameraComponent* GetCameraComponent() const;
	CCameraComponent* GetCameraComponentEdit() const;

	CCameraActor();
	virtual ~CCameraActor();

private:

	using Super = CActor;

private:

	ObjectPtr(CTransformComponent) m_TransformComponent = nullptr;
	ObjectPtr(CCameraComponent) m_CameraComponent = nullptr;

};

BIFROST_NAMESPACE_END