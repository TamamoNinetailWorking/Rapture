#include "ActorSubsystem.h"
#include <Bifrost/Actor/ActorManager.h>

#include <eden/include/utility/ender_utility.h>

USING_BIFROST;

CActorSubsystem::CActorSubsystem()
{
}

CActorSubsystem::~CActorSubsystem()
{
}

bool CActorSubsystem::Initialize(const FSubsystemInitializerBase* _Initializer)
{
	do
	{
		m_ActorManager = new CActorManager();
		CHECK_RESULT_BREAK(m_ActorManager);

		CHECK_RESULT_BREAK(m_ActorManager->Initialize());

		return true;
	} while (0);

	Finalize();
	return false;
}

void CActorSubsystem::Finalize()
{
	EDENS_NAMESPACE::FinalizeObject(m_ActorManager);
}

void CActorSubsystem::DeleteActor(const Hash160& _Name)
{
	CHECK(m_ActorManager);
	m_ActorManager->DeleteActor(_Name);
}

void CActorSubsystem::DeleteActor(const CActor* _Actor)
{
	CHECK(m_ActorManager);
	m_ActorManager->DeleteActor(_Actor);
}

void CActorSubsystem::DestroyActors()
{
	m_ActorManager->DestroyActors();
}

const CActorManager* CActorSubsystem::GetsActorManager() const
{
	return m_ActorManager;
}

CActorManager* CActorSubsystem::GetActorManagerEdit() const
{
	return m_ActorManager;
}
