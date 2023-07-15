#pragma once

#include <Bifrost/Subsystem/SubsystemBase/SubsystemBase.h>

BIFROST_NAMESPACE_BEGIN

class CActorManager;
class CActor;

struct FSubsystemInitializerBase;
struct FActorInitializerBase;

class CActorSubsystem : public CSubsystemBase
{
public:

	bool Initialize(const FSubsystemInitializerBase* _Initializer = nullptr);
	void Finalize();

	template <class Actor>
	Actor* CreateActor(const FActorInitializerBase* _Initializer = nullptr);

	template<class Actor>
	Actor* SearchActor(const Hash160& _Name);

	void DeleteActor(const Hash160& _Name);
	void DeleteActor(const CActor* _Actor);

	void DestroyActors();

	// Actorに対してメッセージを送信するようなシステムもここに載せられそう

	const CActorManager* GetsActorManager() const;
	CActorManager* GetActorManagerEdit() const;

	CActorSubsystem();
	~CActorSubsystem();

private:

	ObjectPtr(CActorManager) m_ActorManager = nullptr;

};


BIFROST_NAMESPACE_END

#include "ActorSubsystemI.inl"
