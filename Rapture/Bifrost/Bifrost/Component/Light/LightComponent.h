#pragma once

#include <Bifrost/Component/Component.h>
#include <Atlantis/DirectX12/GlueMath.h>

#include "LightComponentHandle.h"

BIFROST_NAMESPACE_BEGIN

struct FComponentInitializerBase;

class CLightComponent : public CComponent
{
public:

	bool Initialize(const FComponentInitializerBase* _Initializer = nullptr) override;
	void Finalize() override;

	void BeginPlay() override;
	void EndPlay() override;

	bool Update(float _DeltaTime) override;

	void SetColor(const ATLANTIS_NAMESPACE::Glue::Vector3& _Color);

	const ATLANTIS_NAMESPACE::Glue::Vector3 GetPosition() const;
	const ATLANTIS_NAMESPACE::Glue::Vector3 GetColor() const;

	const FLightComponentManagerHandle& GetHandle() const;

	CLightComponent();
	virtual ~CLightComponent();

private:

	using Super = CComponent;

	ATLANTIS_NAMESPACE::Glue::Vector3 m_Color = {};

	FLightComponentManagerHandle m_Handle = {};
};

BIFROST_NAMESPACE_END