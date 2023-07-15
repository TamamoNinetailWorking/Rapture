#pragma once

#include <Bifrost/Component/Component.h>
#include <Atlantis/DirectX12/GlueMath.h>
#include <Bifrost/Component/Transform/TransformDefine.h>

BIFROST_NAMESPACE_BEGIN

class CTransformComponent : public CComponent
{
private:

	using Matrix = ATLANTIS_NAMESPACE::Glue::FMatrix;
	using Vector3 = ATLANTIS_NAMESPACE::Glue::Vector3;

public:

	bool Initialize(const FComponentInitializerBase* _Initializer = nullptr) override;
	void Finalize() override;

	void BeginPlay() override;
	void EndPlay() override;

	bool Update(float _DeltaTime) override;

	void SetWorldPosition(const Vector3& _Position);
	void SetRotate(const Vector3& _Rotate);
	void SetScale(const Vector3& _Scale);

	const FTransform& GetTransform() const;

	const Vector3& GetWorldPosition() const;
	const Vector3& GetRotate() const;
	const Vector3& GetScale() const;

	const Matrix GetTransformMatrix() const;

	CTransformComponent();
	virtual ~CTransformComponent();

private:

	using Super = CComponent;

private:

	FTransform m_Transform = {};
	//Matrix m_TransformMatrix = {};

};


BIFROST_NAMESPACE_END