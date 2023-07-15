#pragma once

#include <Bifrost/Actor/Actor.h>

BIFROST_NAMESPACE_BEGIN

struct FActorInitializerBase;

class CBloomEffectComponent;

class CPostEffectActor : public CActor
{
private:
	using Super = CActor;
public:

	virtual bool Initialize(const FActorInitializerBase* _Initializer = nullptr) override;
	virtual void Finalize() override;

	virtual void BeginPlay() override;
	virtual void EndPlay() override;

	virtual bool Update(float _DeltaTime) override;

	CPostEffectActor();
	virtual ~CPostEffectActor();

private:

	ObjectPtr(CBloomEffectComponent) m_BloomPostEffectComponent = nullptr;

};

BIFROST_NAMESPACE_END