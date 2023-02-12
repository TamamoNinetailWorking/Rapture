#pragma once

#include <Bifrost/Actor/Actor.h>

BIFROST_NAMESPACE_BEGIN

struct FActorInitializerBase;

class CTransformComponent;
class CLightComponent;

class CLightActor : public CActor
{
public:

	virtual bool Initialize(const FActorInitializerBase* _Initializer = nullptr) override;
	virtual void Finalize() override;

	virtual void BeginPlay() override;
	virtual void EndPlay() override;

	virtual bool Update(float _DeltaTime) override;

	const CLightComponent* GetLightComponent() const;
	CLightComponent* GetLightComponentEdit() const;

	CLightActor();
	virtual ~CLightActor();

private:

	using Super = CActor;

private:

	ObjectPtr(CTransformComponent) m_TransformComponent = nullptr;
	ObjectPtr(CLightComponent) m_LightComponent = nullptr;
};


BIFROST_NAMESPACE_END