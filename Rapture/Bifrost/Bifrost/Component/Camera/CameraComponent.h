#pragma once

#include <Bifrost/Component/Component.h>
#include <Atlantis/DirectX12/GlueMath.h>

#include <Bifrost/Component/Camera/CameraComponentHandle.h>

BIFROST_NAMESPACE_BEGIN

struct FComponentInitializerBase;

class CCameraComponent : public CComponent
{
public:

	bool Initialize(const FComponentInitializerBase* _Initializer = nullptr) override;
	void Finalize() override;

	void BeginPlay() override;
	void EndPlay() override;

	bool Update(float _DeltaTime) override;

	const ATLANTIS_NAMESPACE::Glue::FMatrix GetViewMatrix() const;
	const ATLANTIS_NAMESPACE::Glue::Vector3 GetPosition() const;

	const FCameraComponentManagerHandle& GetHandle() const;

	CCameraComponent();
	virtual ~CCameraComponent();

private:

	using Super = CComponent;

	FCameraComponentManagerHandle m_Handle = {};

};

BIFROST_NAMESPACE_END