#pragma once

#include <Bifrost/Actor/ActorManager.h>

BIFROST_NAMESPACE_BEGIN

template<class Actor>
inline Actor* CActorSubsystem::CreateActor(const FActorInitializerBase* _Initializer)
{
	return PCast<Actor*>(m_ActorManager->CreateActor<Actor>(_Initializer));
}

template<class Actor>
Actor* CActorSubsystem::SearchActor(const Hash160& _Name)
{
	return PCast<Actor*>(m_ActorManager->SearchActor(_Name));
}

BIFROST_NAMESPACE_END