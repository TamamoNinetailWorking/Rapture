#pragma once


BIFROST_NAMESPACE_BEGIN

class CActorManager;
class CActor;

struct FActorInitializerBase;

class CActorSubsystem
{
public:

	bool Initialize();
	void Finalize();

	template <class Actor>
	CActor* CreateActor(const FActorInitializerBase* _Initializer);

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
