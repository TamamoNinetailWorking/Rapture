#include "LevelBase.h"

#include <Bifrost/Actor/ActorInitializerBase.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Actor/ActorSubsystem.h>



USING_BIFROST;

void CLevelBase::Finalize()
{
	for (auto& elem : m_ActorList)
	{
		elem.second = nullptr;
	}

	m_ActorList.clear();
}

void CLevelBase::DeleteActor(const Hash160& _Name)
{
	CActorSubsystem* subsystem = CSubsystemServiceLocator::GetActorSubsystemEdit();
	CHECK(subsystem);

	subsystem->DeleteActor(_Name);

	m_ActorList.erase(_Name);
}

void CLevelBase::DeleteActor(const CActor* _Actor)
{
	CHECK(_Actor);
	this->DeleteActor(_Actor->GetHash());
}